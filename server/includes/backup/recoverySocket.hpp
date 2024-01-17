#pragma once

#include <socket/tcpSocket.hpp>

#define DEFAULT_PORT 4000

class RecoverySocket : public common::TCPSocket
{
  public:
    RecoverySocket(const std::string &clientAddress, int port) : common::TCPSocket()
    {
        client = gethostbyname(clientAddress.c_str());
        if (client == NULL)
        {
            std::cerr << "ERR: could not get host with name " << client << std::endl;
            exit(errno);
        }

        bzero((char *)&this->clientAddress, sizeof(this->clientAddress));
        this->clientAddress.sin_family = AF_INET;

        bcopy((char *)client->h_addr, (char *)&this->clientAddress.sin_addr.s_addr, client->h_length);
        if (port < 1000)
            this->clientAddress.sin_port = htons(DEFAULT_PORT);
        else
            this->clientAddress.sin_port = htons(port);

        connectToClient();
    }

    ~RecoverySocket()
    {
    }

  private:
    struct sockaddr_in clientAddress;
    struct hostent *client;

    void connectToClient()
    {
        int err = connect(socketId, (struct sockaddr *)&clientAddress, sizeof(clientAddress));
        if (err == -1)
        {
            std::cerr << "ERR: failed to connect to socket client\n\t|=> " << strerror(errno) << std::endl;
            exit(errno);
        }

        std::clog << "LOG: successfully connected to client " << clientAddress.sin_addr.s_addr << ":"
                  << clientAddress.sin_port << std::endl;
    }
};