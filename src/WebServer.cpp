#include "WebServer.h"
#include "Server.h"
#include "WebClientHandler.h"
#include "fileFunctions.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>

WebServer::WebServer(const std::string& port) {
    server = new Server(port);
    WWWPath = GetPath();
    fileTraverse(WWWPath);
    Files = files;
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
            Threads.push_back(std::thread(&WebClientHandler::WaitForClients,
                        *(ClientHandlers.top())));
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

std::string WebServer::GetPath() {

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

bool WebServer::IsInFiles(const std::string& file) {
    for(auto& f : Files) {
        if (WWWPath + "/" + file == f) return true;
    }
    return false;
}

WebServer::~WebServer() {
    delete server;
}
