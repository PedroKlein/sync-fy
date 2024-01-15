#pragma once

#include <socket/tcpSocket.hpp>

#define DEFAULT_PORT 4000
#define QUEUE_SIZE 10
#define BUFFER_SIZE 256

/**
 * @class ClientSocket
 * @brief Represents a client-side TCP socket for connecting to a server.
 * 
 * This class extends the common::TCPSocket class to provide additional
 * functionality for connecting to a server.
 */
class ClientSocket : public common::TCPSocket
{
  public:
    /**
     * @brief Constructor.
     * @param serverAddress The IP address or hostname of the server.
     * @param port The port number to connect to on the server.
     */
    ClientSocket(const std::string &serverAddress, int port);

    /**
    * @brief Destructor.
    * Closes the client socket and releases associated resources.
    */
    ~ClientSocket();

    void changeServerAndReconnect(const std::string &newServerAddress);

  private:
    struct sockaddr_in serverAddress;
    struct hostent *server;

    /**
    * @brief Connects the client socket to the specified server.
    * 
    * This private method is called internally by the constructor to establish
    * a connection to the server.
    */
    void connectToServer();
};