#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <string>

class ClientSocket {
private:
    int socket_;

public:
    ClientSocket(const std::string& addr, const std::string& port);
    int getSocket() { return socket_; }
};

#endif
