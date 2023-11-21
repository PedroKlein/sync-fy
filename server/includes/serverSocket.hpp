#pragma once

#include <jsoncpp/json/json.h>
#include <socket/tcpSocket.hpp>

#define QUEUE_SIZE 5

typedef struct Connection
{
    int socket;
    pthread_t thread;
} Connection;

class ServerSocket : public TCPSocket
{
  public:
    ServerSocket(int port);
    ~ServerSocket();
    void StartListening();

  private:
    socklen_t clientLength;
    struct sockaddr_in serverAddress, clientAddress;

    static void *handleClient(void *arg);
    static void receiveData(int clientSocket);

    struct sockaddr_in newSocketAddress(int port);

    // Connections / Clients
    std::vector<Connection> clientConnections;
};