#include "WebServer.h"
#include "Server.h"
#include "WebClientHandler.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <thread>


std::set<std::string, std::greater<std::string>> files;

int dirTree(const char *pathname, const struct stat* sbuf, int type,
        struct FTW* ftwb) {

    if (type == FTW_F) files.insert(std::string(pathname));

    return 0;
}

void fileTraverse(const std::string& path) {

    int flags = 0;
    if (nftw(path.c_str(), dirTree, 10, flags) == -1) {
        throw "EXCEPTION: file traversal error";
    }
}

WebServer::WebServer(const std::string& port) {

    try {
        server = new Server(port);
        WWWPath = getPath();
        fileTraverse(WWWPath);
        Files = files;
        for (int i = 0; i < 16; ++i) {
            ClientHandlers.push(new WebClientHandler(this));
            //TODO can't assign threads
            Threads.push_back(std::thread(&WebClientHandler::WaitForClients(), ClientHandlers.top()));
            ClientHandlers.top()->SetThread(&(Threads.front()));
        }
    }
    catch (const char* exc) {
        throw exc;
    }
    catch (std::ifstream::failure e) {
        throw e;
    }
}

std::string WebServer::getPath() {

    std::string path;
    std::ifstream configFile;
    configFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        configFile.open("server.conf");
        std::getline(configFile, path);
    }
    catch (std::ifstream::failure e) {
        throw e;
    }

    return path;
}

void WebServer::AddClient(int sock) {
    QueueMutex.lock();
    ClientQueue.push(sock);
    QueueMutex.unlock();
}

void WebServer::AddHandler(int sock) {
    StackMutex.lock();
    ClientHandlers.push(new WebClientHandler(sock, this));
    StackMutex.unlock();
}

bool WebServer::isInFiles(const std::string& file) {
    for(auto& f : Files) {
        if (WWWPath + "/" + file == f) return true;
    }
    return false;
}

void WebServer::Run() {
    
    server->Listen();
    std::string address;
    while (true) {
        int sock = server->Accept(address);
        std::cout << "Connection from: " << address << std::endl; 
        AddClient(sock);
        //WebClientHandler webHandler(sock, this);
        //webHandler.Handle();
        close(sock);
    }
}

int WebServer::GetClient() {
    int sock = 0;
    QueueMutex.lock();
    if (ClientQueue.size() != 0) { 
        sock = ClientQueue.front();
        ClientQueue.pop();
    }
    QueueMutex.unlock();
}

WebServer::~WebServer() {
    delete server;
}
