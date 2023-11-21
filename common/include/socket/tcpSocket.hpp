#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class TCPSocket
{
  public:
    TCPSocket()
    {
        if (socketId != 0)
            close(socketId);

        socketId = socket(AF_INET, SOCK_STREAM, 0);
        if (socketId == -1)
        {
            std::cerr << "ERR: failed to create a socket\n\t|=> " << strerror(errno) << std::endl;
            exit(errno);
        }
    }

    virtual ~TCPSocket()
    {
        close(socketId);
    }

    void send(const std::vector<char> &data) const
    {
        ::send(socketId, data.data(), data.size(), 0);
    }

    std::vector<char> receive(size_t size) const
    {
        std::vector<char> buffer(size);
        ssize_t received = recv(socketId, buffer.data(), size, 0);
        buffer.resize(received);
        return buffer;
    }

  protected:
    int socketId;
};