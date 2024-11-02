#pragma once

#include "tcpSocket.hpp"
#include <thread>

#define DEFAULT_PORT 4000

namespace common
{
class ClientSocket : public TCPSocket
{
  public:
    ClientSocket(const std::string &serverAddress, int port) : TCPSocket()
    {
        server = gethostbyname(serverAddress.c_str());
        if (server == NULL)
        {
            std::cerr << "ERR: could not get host with name " << server << std::endl;
            exit(errno);
        }

        bzero((char *)&this->serverAddress, sizeof(this->serverAddress));
        this->serverAddress.sin_family = AF_INET;

        bcopy((char *)server->h_addr, (char *)&this->serverAddress.sin_addr.s_addr, server->h_length);
        if (port < 1000)
            this->serverAddress.sin_port = htons(DEFAULT_PORT);
        else
            this->serverAddress.sin_port = htons(port);

        connectToServer();
    }

    ~ClientSocket()
    {
    }

    // TODO: maybe this needs a mutex? as well as other uses of socketId
    void changeServerAndReconnect(const std::string &newServerAddress)
    {
        close(socketId);

        socketId = socket(AF_INET, SOCK_STREAM, 0);
        if (socketId == -1)
        {
            std::cerr << "ERR: could not create socket" << std::endl;
            exit(errno);
        }

        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(newServerAddress.c_str(), NULL, &hints, &res) != 0)
        {
            std::cerr << "ERR: could not get address info for " << newServerAddress << std::endl;
            exit(errno);
        }

        serverAddress.sin_addr = ((struct sockaddr_in *)res->ai_addr)->sin_addr;

        freeaddrinfo(res);

        connectToServer();
    }

    void connectToServer()
    {
        int maxAttempts = 3;
        int attempt = 0;
        int err = -1;

        while (attempt < maxAttempts && err == -1)
        {
            err = connect(socketId, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
            if (err == -1)
            {
                std::cerr << "ERR: failed to connect to socket server\n\t|=> " << strerror(errno) << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                attempt++;
            }
        }

        if (err == -1)
        {
            std::cerr << "ERR: failed to connect after " << maxAttempts << " attempts" << std::endl;
            exit(errno);
        }

        std::cout << "LOG: successfully connected to server " << serverAddress.sin_addr.s_addr << ":"
                  << serverAddress.sin_port << std::endl;
    }

    // For local development only
    void closeConnection()
    {
        close(socketId);
    }

  private:
    struct sockaddr_in serverAddress;
    struct hostent *server;
};
} // namespace common