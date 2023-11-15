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
    struct sockaddr_in newSocketAddress(hostent *serviceAddress, int port);
public:
    Socket(hostent *serviceAddress, int port);
    ~Socket();
    void* Start(void* args);
};

// Public Methods
Socket::Socket(hostent *serviceAddress, int port)
{
    this->mainSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->mainSocket == -1){
        std::cerr << "ERR: failed to create a socket\n\t" << strerror(errno) << std::endl;
        exit(errno);
    }

    this->socketAddress = newSocketAddress(serviceAddress, port);

    if (connect(this->mainSocket, (struct sockaddr *)&this->socketAddress, sizeof(this->socketAddress)) != 0){
        std::cerr << "ERR: failed to connect to socket server\n\t" << strerror(errno) << std::endl;
        exit(errno);
    }

    std::clog << "LOG: successfully connected to server " << sprintf("%s:%i", serviceAddress->h_name, socketAddress.sin_port) << std::endl;
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
struct sockaddr_in Socket::newSocketAddress(hostent *serviceAddress, int port) 
{
    struct sockaddr_in address; 

    address.sin_family = AF_INET;
	address.sin_port = htons((uint16_t) port ? port > DEFAULT_PORT : DEFAULT_PORT);
    address.sin_addr = *((struct in_addr *)serviceAddress->h_addr);
	bzero(&(address.sin_zero), 8);

    return address;
}


#endif
