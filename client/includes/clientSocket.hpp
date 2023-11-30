#pragma once

#include <socket/tcpSocket.hpp>

#define DEFAULT_PORT 4000
#define QUEUE_SIZE 10
#define BUFFER_SIZE 256

class ClientSocket : public common::TCPSocket
{
  public:
    ClientSocket(const std::string &serverAddress, int port);
    ~ClientSocket();

  private:
    struct sockaddr_in serverAddress;
    struct hostent *server;

    void connectToServer();
};