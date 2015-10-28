#ifndef WEB_CLIENT_HANDLER_H
#define WEB_CLIENT_HANDLER_H

#include "ClientHandler.h"
#include "WebServer.h"

class WebServer;

class WebClientHandler : public ClientHandler {
private:
    WebServer* server;
public:
    WebClientHandler(int sock, WebServer* serv)
        : ClientHandler(sock), server(serv) {}

    void Handle();
};

#endif
