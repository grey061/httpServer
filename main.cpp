#include "ServerSocket.h"
#include "Server.h"

int main() {
    Server server("3490");
    server.ServerLoop();
}
