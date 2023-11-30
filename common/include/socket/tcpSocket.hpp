#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <errno.h>
#include <functional>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace common
{
constexpr size_t DEFAULT_SOCKET_CHUNK_SIZE = 1024 * 4;

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

    void send(const char *buffer, size_t size, size_t chunkSize = DEFAULT_SOCKET_CHUNK_SIZE) const
    {
        ssize_t i = 0;
        while (i < size)
        {
            try
            {
                const int l = ::send(socketId, &buffer[i], std::min(chunkSize, size - i), 0);
                // if (l < 0)
                // {
                //     throw std::runtime_error("Failed to send data");
                // }
                i += l;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Exception during send: " << e.what() << std::endl;
                break;
            }
        }
    }

    void receive(char *buffer, size_t size, size_t chunkSize = DEFAULT_SOCKET_CHUNK_SIZE) const
    {
        size_t i = 0;
        while (i < size)
        {
            try
            {
                const int l = read(socketId, &buffer[i], std::min(chunkSize, size - i));

                if (l == 0)
                {
                    std::cout << "Connection closed" << std::endl;
                    close(socketId);
                    if (onDisconnect)
                    {
                        onDisconnect();
                    }
                    break;
                }
                // else if (l < 0)
                // {
                //     throw std::runtime_error("Failed to receive data");
                // }
                i += l;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Exception during receive: " << e.what() << std::endl;
                break;
            }
        }
    }

    void setOnDisconnect(std::function<void()> callback)
    {
        onDisconnect = callback;
    }

    void checkConnection()
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(socketId, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 0;  // Zero seconds
        timeout.tv_usec = 0; // Zero microseconds

        if (select(socketId + 1, &readfds, NULL, NULL, &timeout) > 0)
        {
            char buffer[1];
            if (read(socketId, buffer, 1) == 0)
            {
                std::cout << "Connection closed" << std::endl;
                close(socketId);
                if (onDisconnect)
                {
                    onDisconnect();
                }
            }
        }
    }

  protected:
    int socketId;
    std::function<void()> onDisconnect;
};
} // namespace common