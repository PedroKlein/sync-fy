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

#define DEFAULT_PORT 8765
#define QUEUE_SIZE 10

class Socket
{
private:
    // Socket vars
    int mainSocket;
    struct sockaddr_in socketAddress; 
    std::vector<int> openSockets[QUEUE_SIZE];
    std::vector<pthread_t> socketThreads;
    std::vector<struct sockaddr_int> clientAddress; 

    // funcs
    struct sockaddr_in newSocketAddress();
    void waitConnections();
    static void* runNewConnection(void* arg); // Static function for thread creation
public:
    Socket();
    ~Socket();
};

Socket::Socket()
{
    this->mainSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->mainSocket == -1)
        std::cerr << "ERR: failed to create a socket\n\t" << strerror(errno) << std::endl;

    this->socketAddress = newSocketAddress();

    if (bind(this->mainSocket, (struct sockaddr *)&this->socketAddress, sizeof(this->socketAddress)) != 0)
        std::cerr << "ERR: failed to bind socket\n\t" << strerror(errno) << std::endl;

    this->waitConnections();
}

Socket::~Socket()
{
}

struct sockaddr_in Socket::newSocketAddress() 
{
    struct sockaddr_in address; 

    address.sin_family = AF_INET;
	address.sin_port = htons(DEFAULT_PORT);
    address.sin_addr.s_addr = INADDR_ANY;
	bzero(&(address.sin_zero), 8);

    return address;
}

// ChatGPT fez essa func sla
void* Socket::runNewConnection(void* arg)
{
    Socket* instance = static_cast<Socket*>(arg); // Cast back to the class instance
    // Code to handle the new connection, protected by mutex if needed
    // For example:
    // pthread_mutex_lock(&(instance->mutex));
    // Perform actions with shared resources
    // pthread_mutex_unlock(&(instance->mutex));
    return NULL;
}

void Socket::waitConnections() 
{
    if (listen(this->mainSocket, QUEUE_SIZE) != 0);
        std::cerr << "ERR: failed to listen\n\t" << strerror(errno) << std::endl;

    while (true)
    {
        socklen_t clilen = sizeof(struct sockaddr_in);
        int newsockfd; 
        if ((newsockfd = accept(this->mainSocket, (struct sockaddr *) &clientAddress, &clilen)) == -1) 
            std::cerr << "ERR: failed to accept connection\n\t" << strerror(errno) << std::endl;

        // Create a new thread for each connection
        pthread_t thread;
        if (pthread_create(&thread, NULL, runNewConnection, this) != 0)
        {
            std::cerr << "ERR: failed to create thread\n\t" << strerror(errno) << std::endl;
            continue; // Continue to accept new connections even if thread creation fails
        }
        socketThreads.push_back(thread);
    }
}



#endif
