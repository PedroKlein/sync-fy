#pragma once

#include <functional>
#include <jsoncpp/json/json.h>
#include <socket/tcpSocket.hpp>

#define QUEUE_SIZE 5

typedef struct Connection
{
    int socket;
    pthread_t thread;
} Connection;

using OnConnectionCallback = std::function<void(int)>;

class ServerSocket : public TCPSocket
{
  public:
    ServerSocket(int port, OnConnectionCallback onClientConnectCallback);
    ~ServerSocket();
    void StartListening();

  private:
    socklen_t clientLength;
    struct sockaddr_in serverAddress, clientAddress;

    struct sockaddr_in newSocketAddress(int port);

    // Connections / Clients
    std::vector<Connection> clientConnections;

    OnConnectionCallback onClientConnectCallback;
};