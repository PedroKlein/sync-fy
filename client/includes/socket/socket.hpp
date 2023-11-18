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
public:
    Socket(char *serverAddress, int port);
    ~Socket();
    void SendData(const /*MyData*/ char* data);

    static void* Start(void* args);
private:
    int clientSocket;
    struct sockaddr_in serverAddress;
    struct hostent *server;

    void connectToServer();
};

// Public Methods
Socket::Socket(char *serverAddress, int port)
{
    if (clientSocket != 0)
        close(clientSocket);

    server = gethostbyname(serverAddress);
    if (server  == NULL) {
        std::cerr << "ERR: could not get host with name " << server << std::endl;
        exit(errno);
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1){
        std::cerr << "ERR: failed to create a socket\n\t|=> " << strerror(errno) << std::endl;
        exit(errno);
    }

    bzero((char *) &this->serverAddress, sizeof(this->serverAddress));
    this->serverAddress.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &this->serverAddress.sin_addr.s_addr, server->h_length);
    if (port < 1000)
        this->serverAddress.sin_port = htons(DEFAULT_PORT);
    else 
        this->serverAddress.sin_port = htons(port);

    connectToServer();
}

Socket::~Socket()
{
    close(clientSocket);
}

void Socket::SendData(const /*MyData*/ char* data) {

    char buffer[BUFFER_SIZE];
    
    while(true) {
        printf("Enter the message: ");
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);

	    /* write in the socket */
	    ssize_t n = write(clientSocket, buffer, strlen(buffer));
        if (n < 0) 
	    	printf("ERROR writing to socket\n");
    
    }
    // // Prepare data to be sent
    // std::ostringstream oss;
    // {
    //     cereal::JSONOutputArchive archive(oss);
    //     archive(data);
    // }

    // // Send data to the server
    // write(clientSocket, oss.str().c_str(), oss.str().length());
    // std::cout << "Data sent to server." << std::endl;
}

// Private Methods
void Socket::connectToServer() {
    int err = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (err == -1){
        std::cerr << "ERR: failed to connect to socket server\n\t|=> " << strerror(errno) << std::endl;
        exit(errno);
    }

    std::clog << "LOG: successfully connected to server " << serverAddress.sin_addr.s_addr << ":" << serverAddress.sin_port << std::endl;
}

#endif
