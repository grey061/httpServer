#ifndef WEB_CLIENT_HANDLER_H
#define WEB_CLIENT_HANDLER_H

#include "ClientHandler.h"
#include "WebServer.h"
#include <thread>

class WebServer;

class WebClientHandler : public ClientHandler {
private:
    WebServer* server;
    std::thread Thread;

public:
    WebClientHandler(int sock, WebServer* serv)
        : ClientHandler(sock), server(serv) {}

    void SetThread(std::thread t) { Thread = t; }
    void WaitForClients() {
        while() {
            
        }
    }

    void Handle();
};

#endif
