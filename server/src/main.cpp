#include "serverSocket.hpp"
#include <constants.hpp>

int main()
{
    ServerSocket commandSocket(COMMAND_PORT);
    ServerSocket serverDataSocket(SERVER_DATA_PORT);
    ServerSocket clientDataSocket(CLIENT_DATA_PORT);

    commandSocket.StartListening();
    serverDataSocket.StartListening();
    clientDataSocket.StartListening();
    return 0;
}