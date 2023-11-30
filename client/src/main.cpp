#include <iostream>

#include "cli/cli.hpp"
#include "cli/commandHandler.hpp"
#include "cli/messageHandler.hpp"
#include "clientMessageHandler.hpp"
#include "clientSocket.hpp"
#include "localMonitor/fileWatcher.hpp"
#include "localMonitor/localMonitor.hpp"
#include "serverMonitor/messageHandler.hpp"
#include "serverMonitor/serverMonitor.hpp"
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
    ClientMessageHandler localMonitorMessageHandler(localMonitorSocket, username);
    localMonitor::FileWatcher &fileWatcher = localMonitor::FileWatcher::getInstance(common::DEFAULT_CLIENT_SYNC_DIR);
    localMonitor::LocalMonitor localMonitor(fileWatcher, localMonitorMessageHandler);
    std::thread *localMonitorThread = localMonitor.start();

    // ServerMonitor
    serverMonitor::MessageHandler serverMonitorMessageHandler(serverMonitorSocket, username);
    serverMonitor::ServerMonitor serverMonitor(serverMonitorMessageHandler);
    std::thread *serverMonitorThread = serverMonitor.start();

    // Finalization
    cliThread->join();
    localMonitorThread->join();
    serverMonitorThread->join();

    return 0;
}
