#ifndef WEB_CLIENT_HANDLER_H
#define WEB_CLIENT_HANDLER_H

#include "ClientHandler.h"
#include "WebServer.h"
#include <thread>

class WebServer;

class WebClientHandler : public ClientHandler {
private:
    WebServer* server;

public:
    WebClientHandler(Server* serv, WebServer* webServ)
        : ClientHandler(serv), server(serv), run(true) {}

    WebClientHandler(int sock, Server* serv, WebServer* webServ)
        : ClientHandler(sock, serv), server(serv), run(true) {}

    void Handle();

    //TODO override destructor
    //~WebClientHandler();
};

#endif
