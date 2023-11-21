#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <errno.h>
#include <jsoncpp/json/json.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_PORT 4000
#define QUEUE_SIZE 10
#define BUFFER_SIZE 256

class Socket
{
  public:
    Socket(char *serverAddress, int port);
    ~Socket();
    void SendData(std::string username) const;

    void StartReceivePingThread();

  private:
    int clientSocket;
    struct sockaddr_in serverAddress;
    struct hostent *server;

    void connectToServer();

    pthread_t receivePingThread;
    static void *receivePing(void *arg);
};