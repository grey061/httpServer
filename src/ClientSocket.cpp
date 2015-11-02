#include "ClientSocket.h"
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <string>

ClientSocket::ClientSocket(const std::string& addr, const std::string& port) {
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(addr.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
        throw "EXCEPTION: getaddrinfo failure";
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((socket_ = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {  
            continue;
        }

        if (connect(socket_, p->ai_addr, p->ai_addrlen) == -1) {
            continue;
        }

        break;
    }

    if (p == NULL) {
        throw "EXCEPTION: failed to connect";
    }

    freeaddrinfo(servinfo);
}
