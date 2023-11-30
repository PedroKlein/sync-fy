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
#include <signal.h>

// std::array<common::TCPSocket *, 3> sockets;

// void handle_sigint(int sig)
// {
//     std::cout << "Caught signal " << sig << std::endl;
//     for (common::TCPSocket *socket : sockets)
//     {
//         if (socket != nullptr)
//         {
//             socket->closeConnection();
//         }
//     }
//     exit(sig);
// }

int main(int argc, char *argv[])
{

    // struct sigaction sigIntHandler;
    // sigIntHandler.sa_handler = handle_sigint;
    // sigemptyset(&sigIntHandler.sa_mask);
    // sigIntHandler.sa_flags = 0;

    // sigaction(SIGINT, &sigIntHandler, NULL);

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

    // sockets = {&commandSocket, &serverMonitorSocket, &localMonitorSocket};

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

    // Handler exit/disconnect
    commandSocket.setOnDisconnect([&cli]() {
        cli.stop();
        std::cout << "Disconnected command socket from server." << std::endl;
    });

    localMonitorSocket.setOnDisconnect([&localMonitor]() {
        localMonitor.stop();
        std::cout << "Disconnected local monitor socket from server." << std::endl;
    });

    serverMonitorSocket.setOnDisconnect([&serverMonitor]() {
        serverMonitor.stop();
        std::cout << "Disconnected server monitor socket from server." << std::endl;
    });

    // Finalization
    cliThread->join();
    localMonitorThread->join();
    serverMonitorThread->join();

    return 0;
}
