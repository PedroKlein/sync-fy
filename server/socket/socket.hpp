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
#define QUEUE_SIZE 5
#define BUFFER_SIZE 256

typedef struct Connection {
    int socket;
    pthread_t thread;
} Connection;

class Socket
{
public:
    Socket(int port);
    ~Socket();
    void StartListening();

private:
    // Server 
    int serverSocket;
    socklen_t clientLength; 
    struct sockaddr_in serverAddress, clientAddress;

    static void* handleClient(void *arg);
    static void receiveData(int clientSocket);

    struct sockaddr_in newSocketAddress(int port);

    // Connections / Clients
    std::vector<Connection> clientConnections;
};

Socket::Socket(int port)
{
    // Socket creation
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1){
        std::cerr << "ERR: failed to create a socket\n\t" << strerror(errno) << std::endl;
        exit(errno);
    }

    // Setting up server address 
    serverAddress = newSocketAddress(port);

    // Binding create socket 
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0){
        std::cerr << "ERR: failed to bind socket\n\t" << strerror(errno) << std::endl;
        exit(errno);
    }

    // Listening for incoming connections
    listen(serverSocket, QUEUE_SIZE);
    std::cout << "Server listening on port " << port << "..." << std::endl;

    clientLength = sizeof(clientAddress);
}

Socket::~Socket()
{
    close(serverSocket);
}

void Socket::StartListening() {
    Connection connection;
    while (true) {
        // Accept a client connection
        connection.socket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientLength);
        if (connection.socket < 0) {
            std::cerr << "Error on accept." << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "Client {" << connection.socket << "} connected." << std::endl;

        // Create a new thread to handle the client
        if (pthread_create(&connection.thread, NULL, handleClient, (void *)&connection.socket) != 0) {
            std::cerr << "Error creating thread." << std::endl;
            exit(EXIT_FAILURE);
        }

        clientConnections.push_back(connection);
    }
}


/*
    === Private Funcs ===
*/

void* Socket::handleClient(void* arg) {
    int clientSocket = *((int *) arg);
    // Maybe a loop for receiving and sending data here,
    // or we should create another thread just for sending data to the clients
    receiveData(clientSocket);
    
    close(clientSocket);
    pthread_exit(NULL);
}

void Socket::receiveData(int clientSocket) {
    char buffer[BUFFER_SIZE];
    while(true) {
        bzero(buffer, BUFFER_SIZE);
        ssize_t readBytes = read(clientSocket, buffer, BUFFER_SIZE);
        if (readBytes > 0)
            std::cout << "Received Data from client " << clientSocket << "\nData: " << buffer << std::endl; 
        // // Deserialize received data
        // MyData receivedData;
        // std::istringstream iss(buffer);
        // {
        //     cereal::JSONInputArchive archive(iss);
        //     archive(receivedData);
        // }

        // std::cout << "Received Data from client: id = " << receivedData.id << ", message = " << receivedData.message << std::endl;
    }
}

struct sockaddr_in Socket::newSocketAddress(int port) 
{
    struct sockaddr_in serverAddress; 

    // Set up server address struct
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    return serverAddress;
}




#endif
