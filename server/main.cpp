#include "socket/socket.hpp"

int main()
{
    Socket socket(DEFAULT_PORT);
    socket.StartPingThread();
    socket.StartListening();
    return 0;
}