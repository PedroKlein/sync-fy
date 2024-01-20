#include "backup/backupState.hpp"
#include "backup/bullyElection.hpp"
#include "backup/primaryMonitor.hpp"

#include "primary/backupConnection/backupConnectionHandler.hpp"
#include "primary/clientConnection/clientConnectionHandler.hpp"
#include <constants.hpp>
#include <iostream>
#include <socket/clientSocket.hpp>
#include <socket/serverSocket.hpp>

// for connecting with primary and receive updates on new clients data
// connected clients, and connected backups
#define BACKUP_SOCKET_PORT 8769

void initializePrimary(int serverId = 0, bool wasBackup = false)
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
    backupConnectionHandler.setServerId(serverId);

    // Ungly gambiarra to solve circular dependency
    BackupState &backupState = BackupState::getInstance();
    backupState.setUpdatedConnectedClientIpsCallback(
        std::bind(&BackupConnectionHandler::hasUpdatedConnectedClientIps, &backupConnectionHandler));
    backupState.setUpdatedConnectedBackupNodesCallback(
        std::bind(&BackupConnectionHandler::hasUpdatedConnectedBackupNodes, &backupConnectionHandler));

    common::ServerSocket backupSocket(BACKUP_SOCKET_PORT);
    std::thread backupSocketThread(&common::ServerSocket::startListening, &backupSocket,
                                   BackupConnectionHandler::onBackupSocketConnection);

    commandSocketThread.join();
    serverDataSocketThread.join();
    clientDataSocketThread.join();

    backupSocketThread.join();
}

int main(int argc, char *argv[])
{
    const bool isPrimary = argc == 1;

    if (isPrimary)
    {
        std::cout << "Starting as primary" << std::endl;

        initializePrimary();

        return 0;
    }

    // Backup section
    int serverId; // TODO: get server id from primary
    std::cout << "Starting as backup" << std::endl;
    std::string primaryServerAddress = argv[1];

    backup::BackupState backupState;
    backupState.primaryServerAddress = primaryServerAddress;

    // 1. Connect to primary server
    common::ClientSocket backupSocket(primaryServerAddress, BACKUP_SOCKET_PORT);
    BackupMessageHandler backupMessageHandler(backupSocket);

    backupMessageHandler.setConnectedIpsCallback([&backupState](const std::vector<std::string> &connectedIps) {
        backupState.connectedClientIps = connectedIps;
    });

    backupMessageHandler.setConnectedNodesCallback([&backupState](const std::vector<common::Node> &connectedNodes) {
        backupState.connectedBackupNodes = connectedNodes;
    });

    // Election section
    backup::BullyElection bullyElection(backupState);

    // 2. Receive data from primary server of next backup and data from clients
    backup::PrimaryMonitor primaryMonitor(backupMessageHandler);
    std::thread *primaryMonitorThread = primaryMonitor.start();

    backupSocket.setOnDisconnect([&primaryMonitor]() {
        std::cout << "Primary disconnected" << std::endl;
        primaryMonitor.stop();

        bullyElection.startElection();
    });

    primaryMonitorThread->join();

    return 0;
}