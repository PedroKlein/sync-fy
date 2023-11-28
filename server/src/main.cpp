#include "connectionHandler.hpp"
#include "serverSocket.hpp"
#include <constants.hpp>

int main()
{
    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
    // TODO: Change callback connection to each socket type
    ServerSocket commandSocket(common::COMMAND_PORT, ConnectionHandler::onCommandSocketConnection);
    ServerSocket serverDataSocket(common::SERVER_DATA_PORT, ConnectionHandler::onCommandSocketConnection);
    ServerSocket clientDataSocket(common::CLIENT_DATA_PORT, ConnectionHandler::onClientDataSocketConnection);

    // start listening inside a thread
    std::thread commandSocketThread(&ServerSocket::startListening, &commandSocket);
    std::thread serverDataSocketThread(&ServerSocket::startListening, &serverDataSocket);
    std::thread clientDataSocketThread(&ServerSocket::startListening, &clientDataSocket);

    commandSocketThread.join();
    serverDataSocketThread.join();
    clientDataSocketThread.join();

    return 0;
}