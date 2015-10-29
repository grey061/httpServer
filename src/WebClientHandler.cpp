#include "WebClientHandler.h"
#include "httpProcessing.h"
#include <string>

std::string httpResponseOKK= "HTTP/1.1 200 OK\n\nHello!";
std::string httpResponseOK= "HTTP/1.1 200 OK\n\n";
std::string httpResponseNotFound = "HTTP/1.1 404 Not Found\n\nDon't have that :(";
std::string httpResponseBadRequest = "HTTP/1.1 400 Bad Request\n\n"
                    "Had some problems receiving your request :(";


// TODO make it wait for further requests for some time
void WebClientHandler::Handle() {
    int bytesRcv;
    int bytesSent;
    std::string rcvMsg;

    rcvMsg = Receive(600, bytesRcv);
    std::cout << "Received " << bytesRcv << " bytes\n";
    std::cout << "MSG:\n" << rcvMsg << "\n"; 
    if (bytesRcv == -1) {
        bytesSent = Send(httpResponseBadRequest);
    }
    else if (parseGET(rcvMsg) == "") {
        bytesSent = Send(httpResponseNotFound);
    }
    else {
        if (server->isInFiles(parseGET(rcvMsg))) {
            bytesSent = Send(httpResponseOK + loadSite(server->GetWWWPath() +
                        "/" + parseGET(rcvMsg)));
        }
        else bytesSent = Send(httpResponseNotFound);
    }
    std::cout << "Bytes sent: " << bytesSent << "\n"; 
    close(ClientSocket);
    std::cout << "Closed connection" << std::endl;
}
