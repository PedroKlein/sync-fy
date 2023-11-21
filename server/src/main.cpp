#include "commandMessageHandler.hpp"
#include "connectionHandler.hpp"
#include "serverSocket.hpp"
#include <constants.hpp>

int main()
{
    ServerSocket commandSocket(COMMAND_PORT, ConnectionHandler::onCommandSocketConnection);
    ServerSocket serverDataSocket(SERVER_DATA_PORT, ConnectionHandler::onCommandSocketConnection);
    ServerSocket clientDataSocket(CLIENT_DATA_PORT, ConnectionHandler::onCommandSocketConnection);

    commandSocket.StartListening();
    serverDataSocket.StartListening();
    clientDataSocket.StartListening();
    return 0;
}