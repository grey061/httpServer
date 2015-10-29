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
    std::string WWWPath;
    std::set<std::string, std::greater<std::string>> Files;

    //TODO use pointers as structure values
    std::stack<WebClientHandler*> ClientHandlers;
    std::mutex StackMutex;

    std::queue<int> ClientQueue;
    std::mutex QueueMutex;

    std::vector<std::thread> Threads;

    std::string getPath();
    void AddClient(int sock);
    void AddHandler(int sock);

public:
   WebServer(const std::string& port); 

   bool isInFiles(const std::string& file);

   void Run();

   std::string GetWWWPath() { return WWWPath; }

   int GetClient();

   ~WebServer();
};

#endif
