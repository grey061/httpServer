#ifndef WEB_CLIENT_HANDLER_H
#define WEB_CLIENT_HANDLER_H

#include "ClientHandler.h"
#include "WebServer.h"

class WebServer;

class WebClientHandler : public ClientHandler {
private:
    WebServer* server;
public:
    WebClientHandler(int sock) : ClientHandler(sock) {}
    void SetServer(WebServer* serv) { server = serv; }
    void Handle();
};

#endif
