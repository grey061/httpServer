#include "ClientHandlerPool.h"
#include "ClientHandler.h"
#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <utility>

typedef std::pair<std::unique_ptr<ClientHandler>,
        std::unique_ptr<std::thread>> ThreadHandlerPair;

ClientHandlerPool::ClientHandlerPool(int handlerCount, Server* server) {
   for (int i = 0; i < handlerCount; ++i) {
       addHandler(server);
   } 
}

//TODO needs some love
bool ClientHandlerPool::turnOn(int i){
    if (i >= 0 && 
        i < handlerCount() && 
        !handlers_[i].first->isOn() && 
        !handlers_[i].first->isHandlingRequest()) {

        handlers_[i].second = std::unique_ptr<std::thread>(
                new std::thread(&ClientHandler::waitForClients, &(*(handlers_[i].first))));
        return true;
    }
    return false;
}

void ClientHandlerPool::turnOff(int i) {
    if (i >= 0 && i < handlerCount() && handlers_[i].first->isOn()) {
        handlers_[i].first->turnOff();
        handlers_[i].second->join();
    }
}

void ClientHandlerPool::shutDown() {
    for (auto& handler : handlers_) {
        if (handler.first->isOn()) {
            handler.first->turnOff();
            handler.second->join();
        }
    }
    std::cout << "Shutdown complete" << std::endl;
}

void ClientHandlerPool::addHandler(Server* server) {
   handlers_.push_back(ThreadHandlerPair(
               std::unique_ptr<ClientHandler>(nullptr), std::unique_ptr<std::thread>(nullptr)));
   handlers_.back().first = std::unique_ptr<ClientHandler>(new ClientHandler(server));
   handlers_.back().second = std::unique_ptr<std::thread>(
           new std::thread(&ClientHandler::waitForClients, &(*(handlers_.back().first))));
}

//TODO make it non-blocking
void ClientHandlerPool::removeHandler() {
    handlers_.back().first->turnOff();
    handlers_.back().second->join();
    handlers_.pop_back();
}

