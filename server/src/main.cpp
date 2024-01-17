#include "backup/recoverySocket.hpp"
#include "primary/connectionHandler.hpp"
#include "serverSocket.hpp"
#include <constants.hpp>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

void initializePrimary()
{
    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();

    ServerSocket commandSocket(common::COMMAND_PORT, ConnectionHandler::onCommandSocketConnection);
    ServerSocket serverDataSocket(common::SERVER_DATA_PORT, ConnectionHandler::onServerDataSocketConnection);
    ServerSocket clientDataSocket(common::CLIENT_DATA_PORT, ConnectionHandler::onClientDataSocketConnection);

    // start listening inside a thread
    std::thread commandSocketThread(&ServerSocket::startListening, &commandSocket);
    std::thread serverDataSocketThread(&ServerSocket::startListening, &serverDataSocket);
    std::thread clientDataSocketThread(&ServerSocket::startListening, &clientDataSocket);

    commandSocketThread.join();
    serverDataSocketThread.join();
    clientDataSocketThread.join();
}

int main(int argc, char *argv[])
{

    if (argc > 1)
    {
        // This is the backup mode
        std::string primaryServerAddress = argv[1];
    }

    // It gets here either by election or by starting as the primary
    initializePrimary();

    return 0;
}