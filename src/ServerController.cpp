#include "ServerController.h"
#include "ClientSocket.h"
#include "ClientHandler.h"
#include <iostream>
#include <memory>
#include <thread>

class ControllerHandler : public ClientHandler {
private:
    ServerController* controller_;

public:
    ControllerHandler(Server* serv, ServerController* contr) :
        ClientHandler(serv), controller_(contr) {}
    bool processRequest(const std::string& msg) {
        std::cout << "Processing msg" << std::endl;
        if (msg.compare(0, 3, "GET") == 0) {
            if (msg.compare(4, 6, "SERVON") == 0) {
                if (server_->isOn())
                    sendString("ON");
                else 
                    sendString("OFF");
            }
            else {
                sendString("Wrong request");
                return false;
            }
        }
        else if (msg.compare(0, 4, "POST") == 0) {
            if (msg.compare(5, 7, "SERVOFF") == 0) {
                if (server_->isOn()) {
                    server_->turnOff(); 
                    int sock = getSocket();
                    ClientSocket servConn("127.0.0.1", "3490");
                    setSocket(servConn.getSocket());
                    sendString("bye");
                    close(clientSocket_);
                    controller_->getServerThread()->join();
                    setSocket(sock);
                }
                else sendString("Allready off");
            }
            else if (msg.compare(5, 7, "POOLOFF") == 0) {
                controller_->turnOffPool();
            }
            else if (msg.compare(5, 6, "ALLOFF") == 0) {
                close(clientSocket_);
                if (server_->isOn()) {
                    server_->turnOff(); 
                    ClientSocket servConn("127.0.0.1", "3490");
                    setSocket(servConn.getSocket());
                    sendString("bye");
                    close(clientSocket_);
                    server_->closeSocket();
                    controller_->getServerThread()->join();
                }
                controller_->turnOff();
                return false;
            }
            else{
                 sendString("Wrong request");
                 return false;
            }
        }
        else if (msg.compare(0, 4, "DONE") == 0)
            return false;
        else {
            sendString("Wrong request");
            return false;
        }
    

        return true;
    }

    void handle() override {
        int bytes;
        std::string rcvMsg;
        bool cont = true;
        while (cont) {
            rcvMsg = receiveString(500, bytes); 
            std::cout << "MSG:" << rcvMsg << std::endl;
            cont = processRequest(rcvMsg);
        }
        close(clientSocket_);
    }
};

void ServerController::run() {
    controllerServer_->waitForClients();
    isOn_ = true;
    int client;
    std::string addr;
    ControllerHandler handler(server_, this);
    
    std::cout << "Controller is running" << std::endl;

    while (isOn_) {
        if ((client = controllerServer_->acceptClient(addr)) != -1) {
            std::cout << "Connection: " << addr << std::endl;
            handler.setSocket(client);
            std::cout << "Socket: " << client << std::endl;
            handler.handle();
        }
    }
    controllerServer_->closeSocket();
    handlerPool_->shutDown();
    std::cout << "Bye..." << std::endl;
}

std::unique_ptr<std::thread> ServerController::runThreaded() {
    return std::unique_ptr<std::thread>
        (new std::thread(&ServerController::run, this));
}
