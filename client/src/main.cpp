#include <iostream>

#include "cli/cli.hpp"
#include "cli/commandHandler.hpp"
#include "cli/messageHandler.hpp"
#include "clientSocket.hpp"
#include "localMonitor/fileWatcher.hpp"
#include "localMonitor/localMonitor.hpp"
#include "localMonitor/messageHandler.hpp"
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

    ClientSocket commandSocket("localhost", common::COMMAND_PORT);
    ClientSocket serverMonitorSocket("localhost", common::SERVER_DATA_PORT);
    ClientSocket localMonitorSocket("localhost", common::CLIENT_DATA_PORT);

    // CLI
    cli::MessageHandler commandMessager(commandSocket, username);
    cli::CommandHandler commandHandler(commandMessager);
    cli::CLI cli(commandHandler);
    std::thread *cliThread = cli.start();

    // LocalMonitor
    localMonitor::MessageHandler localMonitorMessageHandler(localMonitorSocket, username);
    localMonitor::FileWatcher fileWatcher(common::DEFAULT_CLIENT_SYNC_DIR);
    localMonitor::LocalMonitor localMonitor(fileWatcher, localMonitorMessageHandler);
    std::thread *localMonitorThread = localMonitor.start();

    // Finalization
    cliThread->join();
    localMonitorThread->join();

    return 0;
}
