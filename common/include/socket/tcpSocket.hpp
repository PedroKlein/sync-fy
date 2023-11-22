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

#define SOCKET_CHUNK_SIZE 1024 * 4

class TCPSocket
{
  public:
    TCPSocket(int socketId) : socketId(socketId)
    {
    }

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

    void send(const char *buffer, size_t size) const
    {
        ssize_t i = 0;
        while (i < size)
        {
            const int l = ::send(socketId, &buffer[i], std::min(static_cast<size_t>(SOCKET_CHUNK_SIZE), size - i), 0);
            if (l < 0)
            {
                throw std::runtime_error("Failed to send data");
            }
            i += l;
        }

        std::cout << "Sent " << i << " bytes" << std::endl;
    }

    void receive(char *buffer, size_t size) const
    {
        size_t i = 0;
        while (i < size)
        {
            const int l = read(socketId, &buffer[i], std::min(static_cast<size_t>(SOCKET_CHUNK_SIZE), size - i));
            if (l < 0)
            {
                std::runtime_error("Failed to receive data");
            }
            i += l;
        }
    }

  protected:
    int socketId;
};