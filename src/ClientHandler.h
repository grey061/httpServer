#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <string>
#include <unistd.h>
#include "Server.h"
#include <atomic>

class ClientHandler {
protected:
    int clientSocket_;
    bool isOn_;
    std::atomic<bool> isHandlingRequest_;
    Server* server_; 

public:
    ClientHandler(Server* server) : 
        clientSocket_(0), 
        isOn_(false), 
        isHandlingRequest_(false), 
        server_(server) {}

    ClientHandler(int clientSocket, Server* server) : 
        clientSocket_(clientSocket),
        isOn_(false),
        isHandlingRequest_(false),
        server_(server) {}

    void turnOff() { isOn_ = false; } 
    bool isOn() { return isOn_; }
    bool isHandlingRequest() { return isHandlingRequest_; }
    std::string receiveString(const int MAXDATASIZE, int& bytes);
    int sendString(const std::string& source, int& sent);
    int sendString(const std::string& source);
    void setSocket(int sock) { clientSocket_ = sock; }
    int getSocket() { return clientSocket_; }
    void waitForClients();
    virtual void handle();
    //TODO destructor that waits for request to be handled
};

#endif
