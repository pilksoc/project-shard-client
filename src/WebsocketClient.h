#pragma once
#include <mongoose/mongoose.h>
#include <string>
#include <vector>

class Message
{
public:
    Message(mg_ws_message *str);
    Message(char *ptr, size_t len);
    ~Message();
    char *ptr;
    size_t len;
};

class WebsocketCallResponse
{
public:
    WebsocketCallResponse(std::vector<Message *> messages);
    ~WebsocketCallResponse();
    std::vector<Message *> messages;
};

class WebsocketClient
{
public:
    WebsocketClient();
    ~WebsocketClient();
    // To call each frame, non-blocking and returns new messages
    WebsocketCallResponse *Poll();
    // do not use, this is a tempory buffer for incoming messages, the ret of Poll() will return all of the messages and remove them from this buffer
    std::vector<Message *> recieveQueue;
    std::vector<Message *> sendQueue;
    bool is_error;
private:
    struct mg_mgr mgr;
    struct mg_connection *c;
};
