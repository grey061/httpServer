#include "ServerSocket.h"
#include "WebServer.h"
#include "ClientHandler.h"
#include <iostream>
#include <fstream>


int main() {

    try {
        WebServer server("3490");
        server.Run();
    }
    catch (const char * exc) {
        std::cout << "EXCEPTION: contructing socket" << std::endl;
        return 1;
    }
    catch (std::ifstream::failure e) {
        std::cout << "Problem reading config file, check if it exists.\n";
        return 1;
    }

}
