#pragma once
#include <mongoose/mongoose.h>
#include <string>

class WebsocketClient
{
public:
    WebsocketClient(std::string remoteAddr);
    ~WebsocketClient();
private:
    struct mg_mgr mgr;
    struct mg_connection *c;
};
