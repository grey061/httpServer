#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <string>
#include <unistd.h>
#include "Server.h"

class Server;

class ClientHandler {
private:
    int ClientSocket;
    Server* server;

public:
    ClientHandler() { };

    ClientHandler(int sock) : ClientSocket(sock) {};

    std::string Receive(const int MAXDATASIZE, int& bytes);

    int Send(const std::string& source);

    bool HandleClient(bool (*handleFunc)(ClientHandler* handler)) {
        return handleFunc(this); 
    }

    void SetSocket(int sock) { ClientSocket = sock; }

    void SetServer(Server* serv) { server = serv; }
    
    int GetSocket() { return ClientSocket; }

    ~ClientHandler() { close(ClientSocket); };
};

#endif
