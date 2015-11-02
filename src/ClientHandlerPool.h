#ifndef CLIENT_HANDLER_POOL_H
#define CLIENT_HANDLER_POOL_H

#include "ClientHandler.h"
#include "Server.h"
#include <utility>

class ClientHandlerPool {
private:
    std::vector<std::pair<std::unique_ptr<ClientHandler>,
        std::unique_ptr<std::thread>>> handlers_;

    // TODO handler factory
public:
    ClientHandlerPool(int handlerCount, Server* server);
    bool turnOn(int i);
    void turnOff(int i);
    void shutDown();
    void addHandler(Server* server);
    int handlerCount() { return handlers_.size(); }
    void removeHandler();
    ~ClientHandlerPool() { shutDown(); }
};

#endif
