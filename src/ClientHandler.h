#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <string>
#include <unistd.h>

class ClientHandler {
protected:
    int ClientSocket;

public:
    ClientHandler() : ClientSocket(0) {}
    ClientHandler(int sock) : ClientSocket(sock) {}

    std::string Receive(const int MAXDATASIZE, int& bytes);

    int Send(const std::string& source);

    bool HandleClient(bool (*handleFunc)(ClientHandler* handler)) {
        return handleFunc(this); 
    }

    void SetSocket(int sock) { ClientSocket = sock; }
    
    int GetSocket() { return ClientSocket; }

    ~ClientHandler() { close(ClientSocket); }
};

#endif
