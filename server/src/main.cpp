#include "backup/bullyElection.hpp"
#include "backup/data/primaryDataMonitor.hpp"
#include "primary/backupConnection/backupConnectionHandler.hpp"
#include "primary/clientConnection/clientConnectionHandler.hpp"
#include <constants.hpp>
#include <cstdlib>
#include <iostream>
#include <socket/clientSocket.hpp>
#include <socket/serverSocket.hpp>
#include <unistd.h>

// for connecting with primary and receive updates on new clients data
// connected clients, and connected backups
#define BACKUP_SOCKET_PORT 8769

void initializePrimary(int serverId = 0)
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

    // TODO: For primary server
    // 1. Listen for backup server connections

    // 2. Manage backups, connections and disconnections (keep a list of them and update them with their pairs to cloase
    // the ring for the election) keeps pinging them with a ALIVE message

    // 3. After a change on a client data, send the new data to the backup servers
    common::ServerSocket backupData(BACKUP_DATA_SOCKET_PORT);
    std::thread backupDataSocketThread(&common::ServerSocket::startListening, &backupData,
                                       BackupConnectionHandler::onBackupDataSocketConnection);

    commandSocketThread.join();
    serverDataSocketThread.join();
    clientDataSocketThread.join();

    backupDataSocketThread.join();
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

    std::cout << "Starting as backup" << std::endl;
    std::string primaryServerAddress = argv[1];

    // TODO: For backup server

    // 1. Connect to primary server
    common::ClientSocket backupSocket(primaryServerAddress, BACKUP_SOCKET_PORT);
    BackupMessageHandler dataMessageHandler(backupSocket);

    // 2. Receive data from primary server of next backup and data from clients
    backup::PrimaryDataMonitor primaryDataMonitor(dataMessageHandler);
    std::thread *primaryDataMonitorThread = primaryDataMonitor.start();

    // add callback for disconnection
    primaryDataMonitorThread->join();

    // 3. If primary fails, run bully election

    // 4. If elected, run as primary (initializePrimary) and connect to all clients

    // 5. If not elected, repeat from step 1

    common::ClientSocket recoverySocket("localhost", common::CLIENT_RECOVERY_PORT);

    return 0;
}