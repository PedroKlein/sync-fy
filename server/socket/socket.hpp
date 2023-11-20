#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

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

#define DEFAULT_PORT 8765
#define QUEUE_SIZE 5
#define BUFFER_SIZE 256

typedef struct Connection
{
    int socket;
    pthread_t thread;
} Connection;

class Socket
{
  public:
    Socket(int port);
    ~Socket();
    void StartListening();
    void StartPingThread();

  private:
    // Server
    int serverSocket;
    socklen_t clientLength;
    struct sockaddr_in serverAddress, clientAddress;

    static void *handleClient(void *arg);
    static void receiveData(int clientSocket);

    // Flag to control the ping thread
    bool pingThreadRunning;
    pthread_t pingThread;
    static void *pingClients(void *arg);

    struct sockaddr_in newSocketAddress(int port);

    // Connections / Clients
    std::vector<Connection> clientConnections;
};

Socket::Socket(int port)
{
    // Socket creation
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "ERR: failed to create a socket\n\t" << strerror(errno) << std::endl;
        exit(errno);
    }

    // Setting up server address
    serverAddress = newSocketAddress(port);

    // Binding create socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0)
    {
        std::cerr << "ERR: failed to bind socket\n\t" << strerror(errno) << std::endl;
        exit(errno);
    }

    // Listening for incoming connections
    listen(serverSocket, QUEUE_SIZE);
    std::cout << "Server listening on port " << port << "..." << std::endl;

    clientLength = sizeof(clientAddress);
}

Socket::~Socket()
{
    close(serverSocket);
}

void Socket::StartListening()
{
    Connection connection;
    while (true)
    {
        // Accept a client connection
        connection.socket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientLength);
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

void Socket::StartPingThread()
{
    pingThreadRunning = true;
    if (pthread_create(&pingThread, nullptr, &Socket::pingClients, this) != 0)
    {
        std::cerr << "Error creating ping thread." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*
    === Private Funcs ===
*/

void *Socket::handleClient(void *arg)
{
    int clientSocket = *((int *)arg);
    // Maybe a loop for receiving and sending data here,
    // or we should create another thread just for sending data to the clients
    receiveData(clientSocket);

    close(clientSocket);
    pthread_exit(NULL);
}

void Socket::receiveData(int clientSocket)
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

struct sockaddr_in Socket::newSocketAddress(int port)
{
    struct sockaddr_in serverAddress;

    // Set up server address struct
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    return serverAddress;
}

void *Socket::pingClients(void *arg)
{
    Socket *socketInstance = static_cast<Socket *>(arg);

    Json::Value data;
    data["toUsr"] = "";
    data["msg"] = "ping";

    while (socketInstance->pingThreadRunning)
    {
        // Send ping message to each connected client
        for (auto &connection : socketInstance->clientConnections)
        {
            data["toUsr"] = connection.socket;
            std::ostringstream jsonStringStream;
            jsonStringStream << data;

            std::string pingMessage = jsonStringStream.str();

            ssize_t sentBytes = send(connection.socket, pingMessage.c_str(), pingMessage.length(), 0);
            if (sentBytes == -1)
            {
                // Handle error if needed
                std::cerr << "Error sending ping to client " << connection.socket << std::endl;
            }
        }

        // Sleep for a while before sending the next ping
        sleep(5); // Adjust the interval as needed
    }

    pthread_exit(nullptr);
}

#endif
