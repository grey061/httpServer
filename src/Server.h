#ifndef SERVER_H
#define SERVER_H

#include "ServerSocket.h"
#include <string>
#include <queue>
#include <thread>

class Server {
private:
    ServerSocket* socket;
    std::queue<int> RequestQueue;
    std::mutex QueueMutex;
    bool IsOn; 

    void EnqueueRequest(int sock);
    void Listen();
    int Accept(std::string& address);

public:
    Server(const std::string& port);
    void TurnOff() { IsOn = false; }
    int QueueSize() { return RequestQueue.size() }
    int DequeueRequest();
    virtual void Run();
    std::thread Server::RunThreaded();
    virtual ~Server();
};

#endif
