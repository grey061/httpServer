#include <iostream>
#include <memory>
#include <thread>
#include <stdexcept>
#include "ServerSocket.h"
#include "Server.h"
#include "ClientHandler.h"
#include "ClientHandlerPool.h"
#include "ServerController.h"


int main() {
    Server server("3490");
    std::cout << "Created server" << std::endl;

    Server contServ("6666");
    std::cout << "Created controller server" << std::endl;

    ClientHandlerPool pool(3, &server);
    std::cout << "Created handler pool" << std::endl;

    ServerController controller(&contServ, &server, &pool);
    controller.getServerThread() = server.runThreaded();
    std::cout << "Created server controller" << std::endl;
    controller.run();
    //std::unique_ptr<std::thread> contThread = controller.runThreaded();
    //contThread->join();
}
