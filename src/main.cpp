#include "ServerSocket.h"
#include "Server.h"
#include "clientHandler.h"
#include "httpProcessing.h"
#include <iostream>
#include <fstream>

std::string httpResponseOK= "HTTP/1.1 200 OK\n\nHello, World!";
std::string httpResponseNotFound = "HTTP/1.1 404 Not Found\n\nOops, something gone wrong...";

auto func = [] (ClientHandler* thisHandler) -> bool {

    std::string recvMsg;
    int bytesSent; 
    int bytesRecv;

    bytesSent = thisHandler->Send("HELLO\n");
    std::cout << "Sent " << bytesSent << " bytes\n";
    recvMsg = thisHandler->Receive(80, bytesRecv);    
    std::cout << "Received " << bytesRecv << " bytes\n";
    std::cout << "Received message: " << recvMsg << "\n";
    close(thisHandler->GetSocket());
    std::cout << "Connection closed" << std::endl;

    return true;
};

bool myHandler(ClientHandler* thisHandler) {

    std::string recvMsg;
    int bytesSent; 
    int bytesRecv;

    recvMsg = thisHandler->Receive(500, bytesRecv);    
    std::cout << "Received " << bytesRecv << " bytes\n";
    std::cout << recvMsg << std::endl;

    if (parseGET(recvMsg) != "") {
        thisHandler->Send(httpResponseOK);
    }
    else {
        thisHandler->Send(httpResponseNotFound);
    }
    close(thisHandler->GetSocket());
}

int main() {

    try {
        Server server("3490");
        server.handlerFunc = &myHandler;
        server.ServerLoop();
    }
    catch (const char * exc) {
        std::cout << "EXCEPTION: contructing socket" << std::endl;
        return 1;
    }
    catch (std::ifstream::failure e) {
        std::cout << "Problem reading config file, check if it exists.\n";
        return 1;
    }

}
