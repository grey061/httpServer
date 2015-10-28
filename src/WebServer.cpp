#include "WebServer.h"
#include "Server.h"
#include "WebClientHandler.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>


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
            ClientHandlers.push(WebClientHandler(this));
            //TODO run in separate thread
            ClientHandlers.front().
                SetThread(std::thread(&WebClientServer::WaitForClients(), 
                            ClientHandlers.front());
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

void AddClient(int sock) {
    QueueMutex.lock();
    ClientQueue.push(sock);
    QueueMutex.unlock();
}

void AddHandler(int sock) {
    StackMutex.lock();
    ClientHandlers.push(WebClientHandler(sock, this));
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
        WebClientHandler webHandler(sock, this);
        webHandler.Handle();
        close(sock);
    }
}

WebServer::~WebServer() {
    delete server;
}
