#include "connectionHandler.hpp"
#include "serverSocket.hpp"
#include <constants.hpp>

int main()
{
    ConnectionHandler *connectionHandler = ConnectionHandler::getInstance();
    // TODO: Change callback connection to each socket type
    ServerSocket commandSocket(common::COMMAND_PORT, ConnectionHandler::onCommandSocketConnection);
    ServerSocket serverDataSocket(common::SERVER_DATA_PORT, ConnectionHandler::onCommandSocketConnection);
    ServerSocket clientDataSocket(common::CLIENT_DATA_PORT, ConnectionHandler::onClientDataSocketConnection);

    commandSocket.StartListening();
    serverDataSocket.StartListening();
    clientDataSocket.StartListening();
    return 0;
}