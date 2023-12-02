#pragma once

#include <arpa/inet.h>
#include <functional>
#include <jsoncpp/json/json.h>
#include <socket/tcpSocket.hpp>

#define QUEUE_SIZE 5

using OnConnectionCallback = std::function<void(int, std::string)>;

/**
 * @class ServerSocket
 * @brief Class representing a TCP server socket.
 */
class ServerSocket : public common::TCPSocket
{
  public:
    /**
     * @brief Constructor for ServerSocket.
     * @param port The port on which the server socket will listen for incoming connections.
     * @param onClientConnectCallback The callback function to be executed when a client connects.
     */
    ServerSocket(int port, OnConnectionCallback onClientConnectCallback);

    /**
     * @brief Default destructor for ServerSocket.
     */
    ~ServerSocket();

    /**
     * @brief Starts listening for incoming connections.
     */
    void startListening();

    /**
     * @brief Stops listening for incoming connections.
     */
    void stopListening();

  private:
    bool isListening;
    socklen_t clientLength;
    struct sockaddr_in serverAddress, clientAddress;

    /**
     * @brief Creates a new sockaddr_in structure with the specified port.
     * @param port The port for the new sockaddr_in structure.
     * @return The newly created sockaddr_in structure.
     */
    struct sockaddr_in newSocketAddress(int port);

    OnConnectionCallback onClientConnectCallback;
};