#include "backup/bullyElection.hpp"
#include "backup/primaryMonitor.hpp"
#include "backupState.hpp"

#include "primary/backupConnection/backupConnectionHandler.hpp"
#include "primary/clientConnection/clientConnectionHandler.hpp"
#include <constants.hpp>
#include <iostream>
#include <socket/clientSocket.hpp>
#include <socket/serverSocket.hpp>

// for connecting with primary and receive updates on new clients data
// connected clients, and connected backups
#define BACKUP_SOCKET_PORT 8769

void initializePrimary(bool wasBackup = false)
{
    // Handle client connections
    ClientConnectionHandler &connectionHandler = ClientConnectionHandler::getInstance();

    common::ServerSocket commandSocket(common::COMMAND_PORT);
    common::ServerSocket serverDataSocket(common::SERVER_DATA_PORT);
    common::ServerSocket clientDataSocket(common::CLIENT_DATA_PORT);

    // start listening inside a thread
    std::thread commandSocketThread(&common::ServerSocket::startListening, &commandSocket,
                                    ClientConnectionHandler::onCommandSocketConnection);
    std::thread serverDataSocketThread(&common::ServerSocket::startListening, &serverDataSocket,
                                       ClientConnectionHandler::onServerDataSocketConnection);
    std::thread clientDataSocketThread(&common::ServerSocket::startListening, &clientDataSocket,
                                       ClientConnectionHandler::onClientDataSocketConnection);

    // Handle backup connections
    BackupConnectionHandler &backupConnectionHandler = BackupConnectionHandler::getInstance();

    // Ugly gambiarra to solve circular dependency
    BackupState &backupState = BackupState::getInstance();
    backupState.setUpdatedConnectedClientIpsCallback(
        std::bind(&BackupConnectionHandler::hasUpdatedConnectedClientIps, &backupConnectionHandler));
    backupState.setUpdatedConnectedBackupNodesCallback(
        std::bind(&BackupConnectionHandler::hasUpdatedConnectedBackupNodes, &backupConnectionHandler));

    common::ServerSocket backupSocket(BACKUP_SOCKET_PORT);
    std::thread backupSocketThread(&common::ServerSocket::startListening, &backupSocket,
                                   BackupConnectionHandler::onBackupSocketConnection);

    if (wasBackup)
    {
        std::vector<std::string> copyClientIps = backupState.getConnectedIps();

        backupState.setConnectedBackupNodes({});
        backupState.setConnectedClientIps({});

        // This assumes that all clients are still connected, add a tiemeout otherwise
        for (auto &connectedClientIp : copyClientIps)
        {
            common::ClientSocket recoverySocket(connectedClientIp, common::CLIENT_RECOVERY_PORT);
        }
    }

    commandSocketThread.join();
    serverDataSocketThread.join();
    clientDataSocketThread.join();

    backupSocketThread.join();
}

void initializeBackup()
{
    BackupState &backupState = BackupState::getInstance();

    // Backup section
    common::ClientSocket backupSocket(backupState.getPrimaryServerAddress(), BACKUP_SOCKET_PORT);
    BackupMessageHandler backupMessageHandler(backupSocket);

    backupMessageHandler.setConnectedIpsCallback([&backupState](const std::vector<std::string> &connectedIps) {
        backupState.setConnectedClientIps(connectedIps);
    });

    backupMessageHandler.setConnectedNodesCallback([&backupState](const std::vector<common::Node> &connectedNodes) {
        backupState.setConnectedBackupNodes(connectedNodes);
    });

    // Election section
    backup::BullyElection bullyElection;

    backup::PrimaryMonitor primaryMonitor(backupMessageHandler);
    std::thread *primaryMonitorThread = primaryMonitor.start();

    std::thread *electionMonitorThread = bullyElection.listenElections();

    backupSocket.setOnDisconnect([&primaryMonitor, &bullyElection]() {
        std::cout << "Primary disconnected" << std::endl;
        primaryMonitor.stop();

        bullyElection.startElection();
    });

    bullyElection.setElectionEndCallback([&primaryMonitor, &backupState](const std::string &ip) {
        std::cout << "Election ended, new primary is " << ip << std::endl;
        backupState.setPrimaryServerAddress(ip);

        primaryMonitor.stop();

        if (ip == SELF_WIN_IP)
        {
            initializePrimary(true);
        }
    });

    primaryMonitorThread->join();
    electionMonitorThread->join();
}

int main(int argc, char *argv[])
{
    const bool isPrimary = argc == 1;

    BackupState &backupState = BackupState::getInstance();

    if (isPrimary)
    {
        std::cout << "Starting as primary" << std::endl;
        initializePrimary();
        return 0;
    }

    // TODO: get server id from primary
    std::cout << "Starting as backup" << std::endl;
    std::string primaryServerAddress = argv[1];

    backupState.setPrimaryServerAddress(primaryServerAddress);

    while (true)
    {
        initializeBackup();
    }

    return 0;
}