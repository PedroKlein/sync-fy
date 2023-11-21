#include "socket/socket.hpp"
#include <constants.hpp>

int main()
{
    Socket commandSocket(COMMAND_PORT);
    Socket serverDataSocket(SERVER_DATA_PORT);
    Socket clientDataSocket(CLIENT_DATA_PORT);

    commandSocket.StartListening();
    serverDataSocket.StartListening();
    clientDataSocket.StartListening();
    return 0;
}