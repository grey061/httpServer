#ifndef SERVER_H
#define SERVER_H

#include "ServerSocket.h"
#include "clientHandler.h"
#include <string>

class ClientHandler;

class Server {
private:
    ServerSocket* socket;
    std::string WWWpath;
    // TODO Handler Factory

public:
    bool (*handlerFunc)(ClientHandler* thisHandler);

    Server(const std::string& port);

    void Listen();

    int Accept(std::string& address);

    void HandleClient(int clientSocket);

    void ServerLoop();

    ~Server();
};

#endif
