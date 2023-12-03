#pragma once

#include <arpa/inet.h>
#include <functional>
#include <socket/tcpSocket.hpp>

#define QUEUE_SIZE 5

using OnConnectionCallback = std::function<void(int, std::string)>;

class ServerSocket : public common::TCPSocket
{
  public:
    ServerSocket(int port, OnConnectionCallback onClientConnectCallback);
    ~ServerSocket();
    void startListening();
    void stopListening();

  private:
    bool isListening;
    socklen_t clientLength;
    struct sockaddr_in serverAddress, clientAddress;
    struct sockaddr_in newSocketAddress(int port);

    OnConnectionCallback onClientConnectCallback;
};