#include "ClientHandler.h"
#include <string>
#include <iostream>
#include <sys/socket.h>

int ClientHandler::sendString(const std::string& source, int& sent) {
    int total = 0;
    int bytesleft = source.length();
    int n;

    while (total < source.length()) {
        n = send(clientSocket_, source.c_str(), bytesleft, 0);
        if (n == -1) break;
        total += n;
        bytesleft -= n;
    }

    sent = total;

    return n == -1 ? -1 : 0;
}

int ClientHandler::sendString(const std::string& source) {
    int total = 0;
    int bytesleft = source.length();
    int n;

    while (total < source.length()) {
        n = send(clientSocket_, source.c_str(), bytesleft, 0);
        if (n == -1) break;
        total += n;
        bytesleft -= n;
    }

    return n == -1 ? -1 : 0;
}

std::string ClientHandler::receiveString(const int MAXDATASIZE, int& bytes) {
    char *buf = new char[MAXDATASIZE];
    int numbytes;
    std::string retString = "";

    if ((numbytes = recv(clientSocket_, buf, MAXDATASIZE-1, 0)) != -1) {
        buf[numbytes] = '\0';
        retString = std::string(buf);
    }

    bytes = numbytes;
    return retString;
}

void ClientHandler::waitForClients() {
    int client;
    isOn_ = true;
    //std::cout << "Client handler ready for duty!\n";
    while (isOn_) {
        if ((client = server_->waitAndPopClient()) > -1) { 
            setSocket(client);
            isHandlingRequest_ = true;
            handle();
            close(clientSocket_);
            isHandlingRequest_ = false;
        }
    }
}

void ClientHandler::handle() {
    int sent;
    sendString("Someone forgot to override Handle method ;)\n", sent);
}
