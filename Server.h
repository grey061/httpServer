#ifndef SERVER_H
#define SERVER_H

#include "ServerSocket.h"
#include <string>

class Server {
private:
    ServerSocket* socket;
public:
    Server(const std::string& port);
    void Listen();
    int Accept(std::string& address);
    void ServerLoop();
    ~Server();
};

#endif
