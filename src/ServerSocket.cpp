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
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
            throw "EXCEPTION: setsockopt failure";
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
        throw "EXCEPTION: bind failure";
	}
} 

int ServerSocket::getSocket() const {
    return sockfd;
}
