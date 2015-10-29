#include "ClientHandler.h"
#include <string>
#include <sys/socket.h>

int ClientHandler::Send(const std::string& source) {
    return send(ClientSocket, source.c_str(), source.length(), 0);
}

std::string ClientHandler::Receive(const int MAXDATASIZE, int& bytes)
{
    char *buf = new char[MAXDATASIZE];
    int numbytes;
    std::string retString = "";

    if ((numbytes = recv(ClientSocket, buf, MAXDATASIZE-1, 0)) != -1) {
        buf[numbytes] = '\0';
        retString = std::string(buf);
    }

    bytes = numbytes;
    return retString;
}
