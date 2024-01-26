#include <iostream>

#include "cli/cli.hpp"
#include "cli/commandHandler.hpp"
#include "cli/messageHandler.hpp"
#include "clientMessageHandler.hpp"
#include "localMonitor/fileWatcher.hpp"
#include "localMonitor/localMonitor.hpp"
#include "serverMonitor/messageHandler.hpp"
#include "serverMonitor/serverMonitor.hpp"
#include <atomic>
#include <constants.hpp>
#include <mutex>
#include <signal.h>
#include <socket/clientSocket.hpp>
#include <socket/serverSocket.hpp>

void startClient(std::string &username, std::string &serverAddress)
{
    common::ClientSocket commandSocket(serverAddress, common::COMMAND_PORT);
    common::ClientSocket serverMonitorSocket(serverAddress, common::SERVER_DATA_PORT);
    common::ClientSocket localMonitorSocket(serverAddress, common::CLIENT_DATA_PORT);

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

    std::atomic<bool> disconnectHandled(false);

    auto handleDisconnection = [&]() {
        if (disconnectHandled.exchange(true))
        {
            // The handler has already been executed, return immediately
            return;
        }
        std::cout << "ERR: connection to server lost" << std::endl;

        localMonitor.stop();
        serverMonitor.stop();
        cli.stop();
    };

    // Handler exit/disconnect
    commandSocket.setOnDisconnect(handleDisconnection);
    localMonitorSocket.setOnDisconnect(handleDisconnection);
    serverMonitorSocket.setOnDisconnect(handleDisconnection);

    // Finalization
    cliThread->join();
    localMonitorThread->join();
    serverMonitorThread->join();
}

int main(int argc, char *argv[])
{

    std::string username, serverAddress;

#ifdef RELEASE_BUILD
    if (argc < 3)
    {
        std::cerr << "ERR: not enough arguments were provided\n\t";
        std::cerr << "|=> " << argv[0] << " <username> <server_address>" << std::endl;
        exit(errno);
    }

    username = argv[1];
    serverAddress = argv[2];
#else
    username = "chico";
    serverAddress = "localhost";
    // serverAddress = "192.168.0.20";
#endif

    while (true)
    {
        startClient(username, serverAddress);

        std::cout << "Waiting for new server..." << std::endl;

        common::ServerSocket recoverySocket(common::CLIENT_RECOVERY_PORT);
        recoverySocket.startListening([&](int socketId, std::string newServerAddress) {
            serverAddress = newServerAddress;
            recoverySocket.stopListening();
        });
    }

    return 0;
}
