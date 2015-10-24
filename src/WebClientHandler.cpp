#include "WebClientHandler.h"
#include "httpProcessing.h"
#include <string>
#include <fstream>

std::string httpResponseOKK= "HTTP/1.1 200 OK\n\nHello!";
std::string httpResponseOK= "HTTP/1.1 200 OK\n\n";
std::string httpResponseNotFound = "HTTP/1.1 404 Not Found\n\nDon't have that :(";
std::string httpResponseBadRequest = "HTTP/1.1 400 Bad Request\n\nHad some problems receiving your request :(";

std::string LoadFile(std::ifstream &stream)
{
    std::string s;
    stream.seekg(0, std::ios::end);
    s.resize(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(&s[0], s.size());

    return s;
}

std::string loadSite(const std::string& path) {

    std::ifstream site;
    site.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        site.open(path);
        return LoadFile(site);
    }
    catch (std::ifstream::failure e) {
        throw e;
    }
}

void WebClientHandler::Handle() {
    int bytesRcv;
    int bytesSent;
    std::string rcvMsg;

    rcvMsg = Receive(500, bytesRcv);
    if (bytesRcv == -1) {
        Send(httpResponseBadRequest);
    }
    else if (parseGET(rcvMsg) == "") {
        Send(httpResponseNotFound);
    }
    else {
        if (server->isInFiles(parseGET(rcvMsg))) {
            Send(httpResponseOK + loadSite(server->GetWWWPath() + "/" + parseGET(rcvMsg)));
        }
        else Send(httpResponseNotFound);
    }
}