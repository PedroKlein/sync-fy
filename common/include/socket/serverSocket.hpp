#pragma once

#include "tcpSocket.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <functional>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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
        if (pipe(stopPipe) < 0)
        {
            std::cerr << "Error creating pipe." << std::endl;
            return;
        }

        // Listening for incoming connections
        listen(socketId, QUEUE_SIZE);
        std::cout << "Server listening on port " << port << "..." << std::endl;

        clientLength = sizeof(clientAddress);

        isListening = true;

        while (isListening)
        {
            struct pollfd fds[2];
            fds[0].fd = socketId;
            fds[0].events = POLLIN;
            fds[1].fd = stopPipe[0];
            fds[1].events = POLLIN;

            if (poll(fds, 2, -1) < 0)
            {
                std::cerr << "Error on poll." << std::endl;
                break;
            }

            if (fds[1].revents & POLLIN)
            {
                // Stop signal received
                break;
            }

            if (fds[0].revents & POLLIN)
            {
                // Accept a client connection
                int newSocket = accept(socketId, (struct sockaddr *)&clientAddress, &clientLength);
                if (newSocket < 0)
                {
                    std::cerr << "Error on accept." << std::endl;
                    break;
                }

                // Get the client IP
                std::string clientIP(inet_ntoa(clientAddress.sin_addr));
                std::cout << "Client {" << newSocket << "} connected from IP: " << clientIP << std::endl;

                onClientConnectCallback(newSocket, clientIP);
            }
        }

        close(stopPipe[0]);
        close(stopPipe[1]);
    }

    void stopListening()
    {
        isListening = false;

        write(stopPipe[1], "stop", 4);

        close(socketId);
    }

  private:
    int port;
    bool isListening;
    int stopPipe[2];
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