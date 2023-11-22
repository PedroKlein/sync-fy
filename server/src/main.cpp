#include "connectionHandler.hpp"
#include "serverSocket.hpp"
#include <constants.hpp>

int main()
{
    ServerSocket commandSocket(common::COMMAND_PORT, ConnectionHandler::onCommandSocketConnection);
    ServerSocket serverDataSocket(common::SERVER_DATA_PORT, ConnectionHandler::onCommandSocketConnection);
    ServerSocket clientDataSocket(common::CLIENT_DATA_PORT, ConnectionHandler::onCommandSocketConnection);

    commandSocket.StartListening();
    serverDataSocket.StartListening();
    clientDataSocket.StartListening();
    return 0;
}