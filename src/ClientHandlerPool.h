#ifndef CLIENT_HANDLER_POOL_H
#define CLIENT_HANDLER_POOL_H

#include "ClientHandler.h"

class ClientHandlerPool {
private:
    // TODO maybe pairs of ClientHandlers and threads
    std::vector<ClientHandler*> Handlers;
    // TODO handler factory
public:
    // TODO adding deleting handlers
    
};

#endif
