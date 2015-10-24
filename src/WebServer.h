#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "Server.h"
#include <string>
#include <set>
#include <functional>
#include <ftw.h>

class WebServer {
private:
    Server* server;

    std::string WWWPath;

    std::string getPath();

    std::set<std::string, std::greater<std::string>> Files;

public:
   WebServer(const std::string& port); 

   bool isInFiles(const std::string& file);

   void Run();

   std::string GetWWWPath() { return WWWPath; }

   ~WebServer();
};

#endif
