#include "./WebsocketClient.h"
#include <string.h>
#include <testing_h/logger.h>
#include <stdexcept>

static void ws_func(struct mg_connection *c, int ev, void *ev_data)
{
    WebsocketClient *client = (WebsocketClient *) c->fn_data;

    if (ev == MG_EV_OPEN) {
        // TODO: setup TLS
        // } else if (c->is_tls && ev == MG_EV_CONNECT) {
        //     struct mg_str ca = mg_file_read(&mg_fs_posix, s_ca_path);
        //     struct mg_tls_opts opts = {.ca = ca, .name = mg_url_host(s_url)};
        //     mg_tls_init(c, &opts);
    } else if (ev == MG_EV_ERROR) {
        lprintf(LOG_ERROR, "Mongoose error; %p %s\n", c->fd, (char *) ev_data);
    } else if (ev == MG_EV_WS_OPEN) {
        for (auto message : client->sendQueue) {
            mg_ws_send(c, message->ptr, message->len, WEBSOCKET_OP_BINARY);
            delete message;
        }

        client->sendQueue.clear();
    } else if (ev == MG_EV_WS_MSG) {
        struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
        client->recieveQueue.push_back(new Message(wm));
    }

    if (ev == MG_EV_ERROR || ev == MG_EV_CLOSE || ev == MG_EV_WS_MSG) {
        client->is_error = true;
    }
}

WebsocketClient::WebsocketClient()
{
    mg_mgr_init(&this->mgr);
    this->mgr.dns4.url = "udp://1.1.1.1:53";
    this->c = mg_ws_connect(&mgr, "ws://project-shard.leonic.co.uk/ws", &ws_func, this, NULL);
    this->is_error = false;
}

WebsocketClient::~WebsocketClient()
{
    mg_mgr_free(&this->mgr);

    for (auto message : this->recieveQueue) {
        free(message);
    }

    for (auto message : this->sendQueue) {
        free(message);
    }
}

WebsocketCallResponse *WebsocketClient::Poll()
{
    std::vector<Message *> messages(this->recieveQueue);
    this->recieveQueue.clear();
    return new WebsocketCallResponse(messages);
}

WebsocketCallResponse::WebsocketCallResponse(std::vector<Message *> messages)
{
    this->messages = messages;
}

WebsocketCallResponse::~WebsocketCallResponse()
{
    for (auto message : this->messages) {
        free(message);
    }
}

Message::Message(mg_ws_message *str)
{
    char *message = (char *) malloc(sizeof * message * str->data.len);
    if (message == NULL) {
        lprintf(LOG_ERROR, "Cannot allocate message from websocket\n");
        throw std::runtime_error("Malloc error");
    }

    memcpy(message, str->data.buf, str->data.len);
    Message(str->data.buf, str->data.len);
}

Message::Message(char *ptr, size_t len)
{
    this->ptr = ptr;
    this->len = len;
}

Message::~Message()
{
    free(this->ptr);
}
