#include <iostream>

#include "cli/cli.hpp"
#include "cli/commandHandler.hpp"
#include "cli/commandMessager.hpp"
#include "clientSocket.hpp"
#include <constants.hpp>

int main(int argc, char *argv[])
{

    // if (argc < 3)
    // {
    //     std::cerr << "ERR: not enough arguments were provided\n\t";
    //     std::cerr << "|=> " << argv[0] << " <username> <server_address>" << std::endl;
    //     exit(errno);
    // }

    std::string username;

    if (argv[1])
    {
        username = argv[1];
    }
    else
    {
        username = "test";
    }

    ClientSocket commandSocket("localhost", COMMAND_PORT);
    ClientSocket serverDataSocket("localhost", SERVER_DATA_PORT);
    ClientSocket clientDataSocket("localhost", CLIENT_DATA_PORT);

    CommandMessager commandMessager(commandSocket, username);

    CommandHandler handler(commandMessager);
    CLI cli(handler);
    cli.start();

    sleep(100);

    return 0;
}
