#include "Server.h"
#include "ServerSocket.h"
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <fstream>

void *get_in_addr(struct sockaddr *sa) {

	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Server::Server(const std::string& port) {

    try {
        socket = new ServerSocket(port);
    }
    catch (const char * exc) {
        throw exc;
    }
}
void Server::Listen() {

	if (listen(socket->getSocket(), 10) == -1) {
        throw "EXCEPTION: error listening";
	}
}

int Server::Accept(std::string& address) {

    int clientSocket;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];

	sin_size = sizeof (their_addr);
	clientSocket = accept(socket->getSocket(), 
            (struct sockaddr *)&their_addr, &sin_size);

    if (clientSocket != -1) {
	    inet_ntop(their_addr.ss_family,
	    	get_in_addr((struct sockaddr *)&their_addr),
	    	s, sizeof s);
        address = std::string(s);
    }

    return clientSocket;
}

Server::~Server() {
    close(socket->getSocket());
    delete socket;
}
