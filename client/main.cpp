#include <iostream>

#include "includes/cli/cli.hpp"
#include "includes/cli/clientCommandHandler.hpp"
#include "includes/client.hpp"
#include "includes/socket/socket.hpp"
#include <constants.hpp>

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        std::cerr << "ERR: not enough arguments were provided\n\t";
        std::cerr << "|=> " << argv[0] << " <username> <server_address>" << std::endl;
        exit(errno);
    }

    // // Client client(argv[1], argv[2], argv[3]);
    // // client.Start();

    // Socket socket(argv[2], atoi(argv[3]));
    // socket.StartReceivePingThread();
    // socket.SendData(argv[1]);

    Socket commandSocket(argv[2], COMMAND_PORT);
    Socket serverDataSocket(argv[2], SERVER_DATA_PORT);
    Socket clientDataSocket(argv[2], CLIENT_DATA_PORT);

    ClientCommandHandler handler(commandSocket);
    CLI cli(handler);
    cli.start();

    return 0;
}
