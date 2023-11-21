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

ServerSocket::ServerSocket(int port) : TCPSocket()
{
    // Setting up server address
    serverAddress = newSocketAddress(port);

    // Binding create socket
    if (bind(socketId, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0)
    {
        std::cerr << "ERR: failed to bind socket\n\t" << strerror(errno) << std::endl;
        exit(errno);
    }

    // Listening for incoming connections
    listen(socketId, QUEUE_SIZE);
    std::cout << "Server listening on port " << port << "..." << std::endl;

    clientLength = sizeof(clientAddress);
}

ServerSocket::~ServerSocket()
{
}

void ServerSocket::StartListening()
{
    Connection connection;
    while (true)
    {
        // Accept a client connection
        connection.socket = accept(socketId, (struct sockaddr *)&clientAddress, &clientLength);
        if (connection.socket < 0)
        {
            std::cerr << "Error on accept." << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "Client {" << connection.socket << "} connected." << std::endl;

        // Create a new thread to handle the client
        if (pthread_create(&connection.thread, NULL, handleClient, (void *)&connection.socket) != 0)
        {
            std::cerr << "Error creating thread." << std::endl;
            exit(EXIT_FAILURE);
        }

        clientConnections.push_back(connection);
    }
}

void *ServerSocket::handleClient(void *arg)
{
    int clientSocket = *((int *)arg);
    // Maybe a loop for receiving and sending data here,
    // or we should create another thread just for sending data to the clients
    receiveData(clientSocket);

    close(clientSocket);
    pthread_exit(NULL);
}

void ServerSocket::receiveData(int clientSocket)
{
    char buffer[BUFSIZ];
    while (true)
    {
        bzero(buffer, BUFSIZ);
        ssize_t readBytes = read(clientSocket, buffer, BUFSIZ);
        if (readBytes > 0)
        {
            std::cout << "Received Data from client " << clientSocket << "\nData: " << buffer << std::endl;

            // Reconstructing Json Object
            std::string receivedJsonStr(buffer, readBytes);
            Json::CharReaderBuilder reader;
            Json::Value jsonObject;
            std::istringstream jsonStringStream(receivedJsonStr);
            Json::parseFromStream(reader, jsonStringStream, &jsonObject, nullptr);
            std::cout << "Json obj:"
                      << "\n\tuser: " << jsonObject["user"] << "\n\tmsg: " << jsonObject["msg"] << std::endl;
        }
    }
}

struct sockaddr_in ServerSocket::newSocketAddress(int port)
{
    struct sockaddr_in serverAddress;

    // Set up server address struct
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    return serverAddress;
}
