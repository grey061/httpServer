#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <string>

class ServerSocket {
private:
    int socket_;

public:
    ServerSocket() : socket_(-1) {}
    ServerSocket(const std::string& port);
    ServerSocket& operator=(ServerSocket&&);
    ServerSocket(ServerSocket&) = delete;
    int getSocket();
};

#endif
