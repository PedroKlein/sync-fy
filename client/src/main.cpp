#include <iostream>

#include "cli/cli.hpp"
#include "cli/commandHandler.hpp"
#include "cli/messageHandler.hpp"
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

    // ClientSocket commandSocket("localhost", common::COMMAND_PORT);
    // ClientSocket serverDataSocket("localhost", common::SERVER_DATA_PORT);
    // ClientSocket clientDataSocket("localhost", common::CLIENT_DATA_PORT);

    // cli::MessageHandler commandMessager(commandSocket, username);

    // cli::CommandHandler handler(commandMessager);
    // cli::CLI cli(handler);
    // std::thread *cliThread = cli.start();

    // cliThread->join();

    return 0;
}
