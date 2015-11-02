#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H

#include <memory>
#include <thread>
#include "Server.h"
#include "ClientHandlerPool.h"

class ServerController {
private:
    Server* controllerServer_;
    Server* server_;
    ClientHandlerPool* handlerPool_;
    bool isOn_;
    std::unique_ptr<std::thread> serverThread_;

public:
    ServerController(Server* controllerServer, 
            Server* server,
            ClientHandlerPool* pool) :
        controllerServer_(controllerServer),
        server_(server),
        handlerPool_(pool),
        isOn_(false) {}

    std::unique_ptr<std::thread>& getServerThread() { return serverThread_; }
    void turnOff() { isOn_ = false; } 
    void turnOffPool() { handlerPool_->shutDown(); }
    void run();
    std::unique_ptr<std::thread> runThreaded();
};

#endif

