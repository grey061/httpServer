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
        IsOn = false;
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

void Server::EnqueueRequest(int sock) {
    QueueMutex.lock();
    RequestQueue.push(sock);
    QueueMutex.unlock();
}

int Server::DequeueRequest() {
    int request = -1;
    QueueMutex.lock();
    if (RequestQueue.size() != 0) { 
        request = RequestQueue.front();
        RequestQueue.pop();
    }
    QueueMutex.unlock();
    return request;
}

void Server::Run() {
    std::string address = "";
    int sock = 0;

    IsOn = true;

    Listen();

    while (IsOn) {
        if (QueueSize() < 50) {
            sock = server->Accept(address);
            std::cout << "Connection from: " << address << std::endl; 
            if (sock != -1) {
                EnqueueRequest(sock);
            }
        }
    }
}

std::thread Server::RunThreaded() {
    return std::thread(&Server::Run, this);
}

Server::~Server() {
    close(socket->getSocket());
    delete socket;
}
