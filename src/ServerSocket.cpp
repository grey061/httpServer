#include "ServerSocket.h"
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <string>

// TODO replace const char* exceptions with some exception class
ServerSocket::ServerSocket(const std::string& port) {
	struct addrinfo hints, *servinfo, *p;
	int yes = 1;
	int rv;
		
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &servinfo)) != 0) {
        throw "EXCEPTION: gettaddrinfo failure";
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((socket_ = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			continue;
		}

		if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
            throw "EXCEPTION: setsockopt failure";
		}

		if (bind(socket_, p->ai_addr, p->ai_addrlen) == -1) {
			close(socket_);
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
        throw "EXCEPTION: bind failure";
	}
} 

ServerSocket& ServerSocket::operator=(ServerSocket&& rhs) {
    this->socket_ = rhs.socket_;
    return *this;
}

int ServerSocket::getSocket() {
    return socket_;
}
