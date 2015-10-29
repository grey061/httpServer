#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "WebClientHandler.h"
#include "Server.h"
#include <string>
#include <set>
#include <functional>
#include <ftw.h>
#include <thread>
#include <mutex>
#include <stack>
#include <queue>
#include <vector>

class WebClientHandler;

class WebServer {
private:
    Server* server;
    ClientHandlerPool* HandlerPool;
    std::string WWWPath;
    std::set<std::string, std::greater<std::string>> Files;
    std::string GetPath();

public:
    WebServer(const std::string& port); 
    bool IsInFiles(const std::string& file);
    std::string GetWWWPath() { return WWWPath; }
    ~WebServer();
};

#endif
