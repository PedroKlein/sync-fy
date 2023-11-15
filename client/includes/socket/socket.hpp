#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include <iostream>
#include <vector>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <errno.h>

#define DEFAULT_PORT 4000
#define QUEUE_SIZE 10
#define BUFFER_SIZE 256

class Socket
{
private:
    // Socket vars
    int mainSocket;
    struct sockaddr_in socketAddress; 

    

    // funcs
    struct sockaddr_in newSocketAddress(struct hostent *serviceAddress, int port);
public:
    Socket(char *serviceName, char* servicePort);
    ~Socket();
    static void* Start(void* args);
};

// Public Methods
Socket::Socket(char *serviceName, char* servicePort)
{
    struct hostent *serviceAddress = gethostbyname(serviceName);
    if (serviceAddress == NULL) {
        std::cerr << "ERR: could not get host with name " << serviceAddress << std::endl;
        exit(errno);
    }

    int port = atoi(servicePort);

    this->mainSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->mainSocket == -1){
        std::cerr << "ERR: failed to create a socket\n\t|=> " << strerror(errno) << std::endl;
        exit(errno);
    }

    this->socketAddress = newSocketAddress(serviceAddress, port);
    int err = connect(this->mainSocket, (struct sockaddr *)&this->socketAddress, sizeof(this->socketAddress));
    if (err == -1){
        std::cerr << "ERR: failed to connect to socket server\n\t|=> " << strerror(errno) << std::endl;
        exit(errno);
    }

    std::clog << "LOG: successfully connected to server " << serviceAddress->h_name << ":" << socketAddress.sin_port << std::endl;
}

Socket::~Socket()
{
}

void* Socket::Start(void* args) {
    


    while (true) {

        // Check if there is a message to be send 

        // Check if there is a message to be received 

    }

    pthread_exit(NULL);
}

// Private Methods
struct sockaddr_in Socket::newSocketAddress(struct hostent *serviceAddress, int port) 
{
    struct sockaddr_in address; 

    address.sin_family = AF_INET;
    if (port <= 0)
	    address.sin_port = htons((uint16_t) DEFAULT_PORT);
    else 
        address.sin_port = htons((uint16_t) port);
    address.sin_addr = *((struct in_addr *)serviceAddress->h_addr);
	bzero(&(address.sin_zero), 8);

    return address;
}


#endif