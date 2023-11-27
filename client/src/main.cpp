#include <iostream>

#include "cli/cli.hpp"
#include "cli/commandHandler.hpp"
#include "cli/messageHandler.hpp"
#include "fileWatcher/fileWatcher.hpp"
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

    std::string username, dirPath;

    if (argv[1])
    {
        username = argv[1];
    }
    else
    {
        username = "test";
    }

    // CLI
    ClientSocket commandSocket("localhost", common::COMMAND_PORT);
    ClientSocket serverDataSocket("localhost", common::SERVER_DATA_PORT);
    ClientSocket clientDataSocket("localhost", common::CLIENT_DATA_PORT);

    cli::MessageHandler commandMessager(commandSocket, username);

    cli::CommandHandler handler(commandMessager);
    cli::CLI cli(handler);
    std::thread *cliThread = cli.start();

    // FileWatcher
    FileWatcher fileWatcher(DEFAULT_PATH);

    fileWatcher.setFileAddedCallback([handler](const std::string &filePath){ 
        std::cout << "File added: " << filePath << std::endl; 
        handler.executeCommand("upload", std::vector<std::string>{filePath});
    });

    fileWatcher.setFileModifiedCallback([](const std::string &filePath){ 
        std::cout << "File modified: " << filePath << std::endl; 
    });

    fileWatcher.setFileRemovedCallback([](const std::string &filePath){ 
        std::cout << "File removed: " << filePath << std::endl; 
    });

    std::thread *fileWatcherThread = fileWatcher.start();

    // Finalization
    cliThread->join();
    fileWatcherThread->join();

    std::cout << "Hello, World!" << std::endl;

    return 0;
}
