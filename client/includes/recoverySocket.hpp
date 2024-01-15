#pragma once

#include <arpa/inet.h>
#include <functional>
#include <socket/tcpSocket.hpp>

class RecoverySocket : public common::TCPSocket
{
  public:
    RecoverySocket(int port) : common::TCPSocket()
    {
        serverAddress = newSocketAddress(port);

        if (bind(socketId, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0)
        {
            std::cerr << "ERR: failed to bind socket\n\t" << strerror(errno) << std::endl;
            exit(errno);
        }
    }

    ~RecoverySocket()
    {
    }

    std::string getNewAddress()
    {
        listen(socketId, 1);
        std::cout << "Trying to recover..." << std::endl;

        socklen_t clientLength = sizeof(clientAddress);

        int newSocket = accept(socketId, (struct sockaddr *)&clientAddress, &clientLength);
        if (newSocket < 0)
        {
            std::cerr << "Error on accept." << std::endl;
            exit(EXIT_FAILURE);
        }

        std::string clientIP(inet_ntoa(clientAddress.sin_addr));

        return clientIP;
    }

  private:
    struct sockaddr_in serverAddress, clientAddress;

    struct sockaddr_in newSocketAddress(int port)
    {
        struct sockaddr_in serverAddress;

        bzero((char *)&serverAddress, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        return serverAddress;
    }
};