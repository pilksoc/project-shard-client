#include "./WebsocketClient.h"

WebsocketClient::WebsocketClient(std::string remoteAddr)
{
    mg_mgr_init(&mgr);
    // TODO: connect to remote server
}

WebsocketClient::~WebsocketClient()
{
    mg_mgr_free(&this->mgr);
}
