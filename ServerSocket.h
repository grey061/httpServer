#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <string>

class ServerSocket {
private:
    int sockfd;
public:
    ServerSocket(const std::string& port);
    int getSocket() const;
};

#endif
