#include "connectionHandler.hpp"
#include "recoverySocket.hpp"
#include "serverSocket.hpp"
#include <constants.hpp>
#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <unistd.h>

std::array<ServerSocket *, 3> sockets;

void handle_sigint(int sig)
{
    for (ServerSocket *socket : sockets)
    {
        if (socket != nullptr)
        {
            socket->stopListening();
        }
    }
    exit(sig);
}

int main(int argc, char *argv[])
{

    // this is the primary mode
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handle_sigint;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();

    ServerSocket commandSocket(common::COMMAND_PORT, ConnectionHandler::onCommandSocketConnection);
    ServerSocket serverDataSocket(common::SERVER_DATA_PORT, ConnectionHandler::onServerDataSocketConnection);
    ServerSocket clientDataSocket(common::CLIENT_DATA_PORT, ConnectionHandler::onClientDataSocketConnection);

    sockets = {&commandSocket, &serverDataSocket, &clientDataSocket};

    // start listening inside a thread
    std::thread commandSocketThread(&ServerSocket::startListening, &commandSocket);
    std::thread serverDataSocketThread(&ServerSocket::startListening, &serverDataSocket);
    std::thread clientDataSocketThread(&ServerSocket::startListening, &clientDataSocket);

    std::string primaryServerAddress;

    // please future me, refactor this or I will die
    if (argc > 1)
    {
        // this is the backup mode
        primaryServerAddress = argv[1];

        RecoverySocket recoverySocket("localhost", common::CLIENT_RECOVERY_PORT);
    }

    commandSocketThread.join();
    serverDataSocketThread.join();
    clientDataSocketThread.join();

    return 0;
}