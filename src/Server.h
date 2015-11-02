#ifndef SERVER_H
#define SERVER_H

#include "ServerSocket.h"
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <unistd.h>

//class Socket;

class Server {
protected:
    ServerSocket socket_;
    std::queue<int> clientQueue_;
    std::mutex queueMutex_;
    std::condition_variable newClient_;
    bool isOn_; 

    void pushClient(int sock);

public:
    Server(const std::string& port);
    void waitForClients();
    int acceptClient(std::string& address);
    bool isOn() { return isOn_; }
    void turnOff() { isOn_ = false; }
    int popClient();
    int waitAndPopClient();
    virtual void run();
    std::unique_ptr<std::thread> runThreaded();
    void closeSocket() { close(socket_.getSocket()); }
};

#endif
