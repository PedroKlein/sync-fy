#include "serverSocket.hpp"

ServerSocket::ServerSocket(int port, OnConnectionCallback onClientConnectCallback)
    : common::TCPSocket(), onClientConnectCallback(onClientConnectCallback)
{
    // Setting up server address
    serverAddress = newSocketAddress(port);

    // Binding create socket
    if (bind(socketId, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0)
    {
        std::cerr << "ERR: failed to bind socket\n\t" << strerror(errno) << std::endl;
        exit(errno);
    }

    // Listening for incoming connections
    listen(socketId, QUEUE_SIZE);
    std::cout << "Server listening on port " << port << "..." << std::endl;

    clientLength = sizeof(clientAddress);
}

ServerSocket::~ServerSocket()
{
}

void ServerSocket::startListening()
{
    isListening = true;
    do
    {
        // Accept a client connection
        int newSocket = accept(socketId, (struct sockaddr *)&clientAddress, &clientLength);
        if (newSocket < 0)
        {
            std::cerr << "Error on accept." << std::endl;
            exit(EXIT_FAILURE);
        }

        // Get the client IP
        std::string clientIP(inet_ntoa(clientAddress.sin_addr));
        std::cout << "Client {" << newSocket << "} connected from IP: " << clientIP << std::endl;

        onClientConnectCallback(newSocket, clientIP);
    } while (isListening);
}

void ServerSocket::stopListening()
{
    isListening = false;
    close(socketId);
}

struct sockaddr_in ServerSocket::newSocketAddress(int port)
{
    struct sockaddr_in serverAddress;

    // Set up server address struct
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    return serverAddress;
}
