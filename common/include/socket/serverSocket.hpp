#pragma once

#include "tcpSocket.hpp"
#include <arpa/inet.h>
#include <functional>

#define QUEUE_SIZE 5

namespace common
{
using OnConnectionCallback = std::function<void(int, std::string)>;

class ServerSocket : public TCPSocket
{
  public:
    ServerSocket(int port) : TCPSocket(), port(port), isListening(false)
    {
        // Setting up server address
        serverAddress = newSocketAddress(port);

        // Binding create socket
        if (bind(socketId, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0)
        {
            std::cerr << "ERR: failed to bind socket\n\t" << strerror(errno) << std::endl;
            exit(errno);
        }
    }

    ~ServerSocket()
    {
    }

    void startListening(OnConnectionCallback onClientConnectCallback)
    {
        // Listening for incoming connections
        listen(socketId, QUEUE_SIZE);
        std::cout << "Server listening on port " << port << "..." << std::endl;

        clientLength = sizeof(clientAddress);

        isListening = true;
        do
        {
            // Accept a client connection
            int newSocket = accept(socketId, (struct sockaddr *)&clientAddress, &clientLength);
            if (newSocket < 0)
            {
                std::cerr << "Error on accept." << std::endl;
                this->stopListening();
            }

            // Get the client IP
            std::string clientIP(inet_ntoa(clientAddress.sin_addr));
            std::cout << "Client {" << newSocket << "} connected from IP: " << clientIP << std::endl;

            onClientConnectCallback(newSocket, clientIP);
        } while (isListening);
    }

    void stopListening()
    {
        isListening = false;
        close(socketId);
    }

  private:
    int port;
    bool isListening;
    socklen_t clientLength;
    struct sockaddr_in serverAddress, clientAddress;

    struct sockaddr_in newSocketAddress(int port)
    {
        struct sockaddr_in serverAddress;

        // Set up server address struct
        bzero((char *)&serverAddress, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        return serverAddress;
    }
};
} // namespace common