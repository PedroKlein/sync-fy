#include "backup/recoverySocket.hpp"
#include "primary/connectionHandler.hpp"
#include <constants.hpp>
#include <cstdlib>
#include <iostream>
#include <socket/clientSocket.hpp>
#include <socket/serverSocket.hpp>
#include <unistd.h>

void initializePrimary()
{
    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();

    common::ServerSocket commandSocket(common::COMMAND_PORT);
    common::ServerSocket serverDataSocket(common::SERVER_DATA_PORT);
    common::ServerSocket clientDataSocket(common::CLIENT_DATA_PORT);

    // start listening inside a thread
    std::thread commandSocketThread(&common::ServerSocket::startListening, &commandSocket,
                                    ConnectionHandler::onCommandSocketConnection);
    std::thread serverDataSocketThread(&common::ServerSocket::startListening, &serverDataSocket,
                                       ConnectionHandler::onServerDataSocketConnection);
    std::thread clientDataSocketThread(&common::ServerSocket::startListening, &clientDataSocket,
                                       ConnectionHandler::onClientDataSocketConnection);

    commandSocketThread.join();
    serverDataSocketThread.join();
    clientDataSocketThread.join();
}

int main(int argc, char *argv[])
{

    // It gets here either by election or by starting as the primary
    // initializePrimary();

    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();

    common::ServerSocket commandSocket(common::COMMAND_PORT);
    common::ServerSocket serverDataSocket(common::SERVER_DATA_PORT);
    common::ServerSocket clientDataSocket(common::CLIENT_DATA_PORT);

    // start listening inside a thread
    std::thread commandSocketThread(&common::ServerSocket::startListening, &commandSocket,
                                    ConnectionHandler::onCommandSocketConnection);
    std::thread serverDataSocketThread(&common::ServerSocket::startListening, &serverDataSocket,
                                       ConnectionHandler::onServerDataSocketConnection);
    std::thread clientDataSocketThread(&common::ServerSocket::startListening, &clientDataSocket,
                                       ConnectionHandler::onClientDataSocketConnection);

    if (argc > 1)
    {
        // This is the backup mode
        std::string primaryServerAddress = argv[1];

        common::ClientSocket recoverySocket("localhost", common::CLIENT_RECOVERY_PORT);
    }

    commandSocketThread.join();
    serverDataSocketThread.join();
    clientDataSocketThread.join();

    return 0;
}