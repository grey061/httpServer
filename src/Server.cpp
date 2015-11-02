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
#include <condition_variable>
#include <chrono>
#include <memory>
#include <functional>

void *get_in_addr(struct sockaddr *sa) {

	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Server::Server(const std::string& port) {

    try {
        socket_ = ServerSocket(port);
        isOn_ = false;
    }
    catch (const char * exc) {
        throw exc;
    }
}

void Server::waitForClients() {

	if (listen(socket_.getSocket(), 5) == -1) {
        throw "EXCEPTION: error listening";
	}
}

int Server::acceptClient(std::string& address) {

    int clientSocket;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];

	sin_size = sizeof (their_addr);
	clientSocket = accept(socket_.getSocket(), 
            (struct sockaddr *)&their_addr, &sin_size);

    if (clientSocket != -1) {
	    inet_ntop(their_addr.ss_family,
	    	get_in_addr((struct sockaddr *)&their_addr),
	    	s, sizeof s);
        address = std::string(s);
    }

    return clientSocket;
}

void Server::pushClient(int sock) {
    std::lock_guard<std::mutex> lk(queueMutex_);
    clientQueue_.push(sock);
    newClient_.notify_one();
}

int Server::popClient() {
    int client = -1;
    std::lock_guard<std::mutex> lk(queueMutex_);
    if (clientQueue_.size() != 0) { 
        client = clientQueue_.front();
        clientQueue_.pop();
    }
    return client;
}

int Server::waitAndPopClient() {
    int sock = -1;
    std::unique_lock<std::mutex> lk(queueMutex_);
    if (newClient_.wait_for(lk, std::chrono::milliseconds(1000), 
                [this] {return !clientQueue_.empty();})) {
        sock = clientQueue_.front();
        clientQueue_.pop();
    }
    return sock;
}

void Server::run() {
    std::string address = "";
    int sock = 0;
    isOn_ = true;

    waitForClients();

    std::cout << "Server is running" << std::endl;

    while (isOn_) {
        if (clientQueue_.size() < 50) {
            sock = acceptClient(address);
            std::cout << "Connection from: " << address << std::endl; 
            if (sock != -1) {
                pushClient(sock);
            }
        }
    }
    closeSocket();
    std::cout << "Server closed" << std::endl;
}

std::unique_ptr<std::thread> Server::runThreaded() {
    return std::unique_ptr<std::thread>(
            new std::thread(&Server::run, std::ref(*this)));
}
