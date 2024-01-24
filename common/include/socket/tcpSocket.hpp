#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <errno.h>
#include <fcntl.h>
#include <functional>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
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
/**
 * @class TCPSocket
 * @brief Represents a TCP socket for communication 
 */
class TCPSocket
{
  public:
    /**
     * @brief Constructor with socket ID.
     * @param socketId The ID of an existing socket.
     */
    TCPSocket(int socketId) : socketId(socketId)
    {
        // enableTCPKeepAlive();
    }

     /**
     * @brief Default constructor.
     * Creates a new TCP socket.
     */
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
        // enableTCPKeepAlive();
    }

     /**
     * @brief Destructor.
     * Closes the socket.
     */
    virtual ~TCPSocket()
    {
        std::cout << "Connection closed" << std::endl;
        shutdown(socketId, SHUT_RDWR);
        close(socketId);
    }

    /**
     * @brief Sends data over the socket.
     * @param buffer The data to be sent.
     * @param size The size of the data.
     * @param chunkSize The size of each chunk to be sent at a time.
     */
    void send(const char *buffer, size_t size, size_t chunkSize = DEFAULT_SOCKET_CHUNK_SIZE)
    {
        ssize_t i = 0;
        while (i < size)
        {
            try
            {
                if (!isSocketAlive())
                {
                    std::cout << "Socket is dead" << std::endl;
                    closeConnection();
                    break;
                }
                const int l = ::send(socketId, &buffer[i], std::min(chunkSize, size - i), 0);
                if (l < 0 || l == 0)
                {
                    std::cout << "Socket is dead - after send" << std::endl;
                    closeConnection();
                    break;
                }
                i += l;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Exception during send: " << e.what() << std::endl;
                break;
            }
        }
    }

    /**
     * @brief Receives data from the socket.
     * @param buffer The buffer to store the received data.
     * @param size The size of the buffer.
     * @param chunkSize The size of each chunk to be received at a time.
     */
    void receive(char *buffer, size_t size, size_t chunkSize = DEFAULT_SOCKET_CHUNK_SIZE)
    {
        size_t i = 0;
        while (i < size)
        {
            try
            {
                const int l = read(socketId, &buffer[i], std::min(chunkSize, size - i));

                if (l < 0 || l == 0)
                {
                    std::cout << "Socket is dead - after read" << std::endl;
                    closeConnection();
                    break;
                }
                i += l;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Exception during receive: " << e.what() << std::endl;
                closeConnection();
                break;
            }
        }
    }

    /**
    * @brief Closes the socket connection.
    */
    void closeConnection()
    {
        std::cout << "Closing connection" << std::endl;
        close(socketId);
        if (onDisconnect)
        {
            onDisconnect();
        }
    }

    /**
    * @brief Sets the callback function to be called on disconnection.
    * @param callback The callback function.
    */
    void setOnDisconnect(std::function<void()> callback)
    {
        onDisconnect = callback;
    }

    bool isSocketAlive()
    {
        char buffer;
        int flags = fcntl(socketId, F_GETFL, 0);
        fcntl(socketId, F_SETFL, flags | O_NONBLOCK);

        ssize_t result = recv(socketId, &buffer, 1, MSG_PEEK);

        fcntl(socketId, F_SETFL, flags);

        if (result > 0)
        {
            return true;
        }
        else if (result == 0)
        {
            return false;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

  protected:
    int socketId;
    std::function<void()> onDisconnect;

  private:
    void enableTCPKeepAlive()
    {
        int yes = 1;
        if (setsockopt(socketId, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1)
        {
            std::cerr << "ERR: failed to set SO_KEEPALIVE on the socket\n\t|=> " << strerror(errno) << std::endl;
            closeConnection();
            return;
        }

        int keepIdle = 60; // seconds before starting to send keepalive probes
        if (setsockopt(socketId, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int)) == -1)
        {
            std::cerr << "ERR: failed to set TCP_KEEPIDLE on the socket\n\t|=> " << strerror(errno) << std::endl;
            closeConnection();
            return;
        }

        int keepInterval = 10; // interval between keepalive probes
        if (setsockopt(socketId, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int)) == -1)
        {
            std::cerr << "ERR: failed to set TCP_KEEPINTVL on the socket\n\t|=> " << strerror(errno) << std::endl;
            closeConnection();
            return;
        }

        int keepCount = 3; // number of unacknowledged probes to consider connection dead
        if (setsockopt(socketId, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int)) == -1)
        {
            std::cerr << "ERR: failed to set TCP_KEEPCNT on the socket\n\t|=> " << strerror(errno) << std::endl;
            closeConnection();
            return;
        }
    }
};
} // namespace common