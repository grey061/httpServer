#ifndef WEB_CLIENT_HANDLER_H
#define WEB_CLIENT_HANDLER_H

#include "ClientHandler.h"
#include "WebServer.h"
#include <thread>

class WebServer;

class WebClientHandler : public ClientHandler {
private:
    WebServer* server;
    std::thread* Thread;
    bool run;

public:
    WebClientHandler(WebServer* serv)
        : ClientHandler(), server(serv), run(true) {}

    WebClientHandler(int sock, WebServer* serv)
        : ClientHandler(sock), server(serv), run(true) {}

    void SetThread(std::thread* t) { Thread = t; }

    void WaitForClients();

    void Handle();
};

#endif
