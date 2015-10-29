#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <string>
#include <unistd.h>
#include "Server.h"

class ClientHandler {
protected:
    int ClientSocket;
    bool IsOn;
    Server* server;

public:
    ClientHandler(Server* serv) 
        : ClientSocket(0), IsOn(false), server(serv) {}
    ClientHandler(int sock, Server* serv) 
        : ClientSocket(sock), IsOn(false), server(serv) {}
    void TurnOff() { IsOn = false; } 
    std::string Receive(const int MAXDATASIZE, int& bytes);
    int Send(const std::string& source);
    void SetSocket(int sock) { ClientSocket = sock; }
    int GetSocket() { return ClientSocket; }
    void WaitForClients();
    virtual void Handle();
    ~ClientHandler() { close(ClientSocket); }
};

#endif
