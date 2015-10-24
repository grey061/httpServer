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
        WebClientHandler webHandler(sock);
        webHandler.SetServer(this); 
        webHandler.Handle();
        close(sock);
    }
}

WebServer::~WebServer() {
    delete server;
}
