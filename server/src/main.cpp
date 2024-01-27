#include "backup/bullyElection.hpp"
#include "backup/primaryMonitor.hpp"
#include "backupState.hpp"

#include "primary/backupConnection/backupConnectionHandler.hpp"
#include "primary/clientConnection/clientConnectionHandler.hpp"
#include <constants.hpp>
#include <filesystem>
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
        // std::this_thread::sleep_for(std::chrono::seconds(5));
        std::vector<std::string> copyClientIps = backupState.getConnectedIps();

        // std::vector<std::string> copyClientIps = {"192.168.0.16"};

        backupState.setConnectedBackupNodes({});
        backupState.setConnectedClientIps({});

        // This assumes that all clients are still connected, try run it in parallel
        for (auto &connectedClientIp : copyClientIps)
        {
            std::cout << "Trying to connect to client " << connectedClientIp << std::endl;
            common::ClientSocket recoverySocket(connectedClientIp, common::CLIENT_RECOVERY_PORT);
            // std::this_thread::sleep_for(std::chrono::seconds(2));
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

    std::cout << "Trying to connect to primary - " << backupState.getPrimaryServerAddress() << std::endl;

    // Backup section
    common::ClientSocket backupSocket(backupState.getPrimaryServerAddress(), BACKUP_SOCKET_PORT);
    std::cout << "Connected to primary" << std::endl;

    BackupMessageHandler backupMessageHandler(backupSocket);

    backupMessageHandler.setConnectedIpsCallback([&backupState](const std::vector<std::string> &connectedIps) {
        backupState.setConnectedClientIps(connectedIps);
    });

    backupMessageHandler.setConnectedNodesCallback([&backupState](const std::vector<common::Node> &connectedNodes) {
        backupState.setConnectedBackupNodes(connectedNodes);
    });

    // Election section
    backup::BullyElection bullyElection;
    common::ServerSocket electionSocket(ELECTION_SOCKET_PORT);

    backup::PrimaryMonitor primaryMonitor(backupMessageHandler);
    std::thread *primaryMonitorThread = primaryMonitor.start();

    std::thread electionMonitorThread(&common::ServerSocket::startListening, &electionSocket,
                                      [&bullyElection](int clientSocketId, std::string ip) {
                                          bullyElection.onElectionSocketConnection(clientSocketId, ip);
                                      });

    backupSocket.setOnDisconnect([&primaryMonitor, &bullyElection]() {
        std::cout << "Primary disconnected" << std::endl;
        primaryMonitor.stop();
        bullyElection.startElection();
    });

    bullyElection.setElectionEndCallback([&primaryMonitor, &backupState, &electionSocket](const std::string &ip) {
        std::cout << "Election ended, new primary is " << ip << std::endl;
        backupState.setPrimaryServerAddress(ip);

        electionSocket.stopListening();
        // primaryMonitor.stop();
    });

    primaryMonitorThread->join();
    electionMonitorThread.join();
}

int main(int argc, char *argv[])
{
    const bool isPrimary = argc == 1;
    // const bool isPrimary = false;

    try
    {
        std::filesystem::create_directory(common::DEFAULT_SERVER_SYNC_DIR);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to create folder: " << e.what() << std::endl;
        exit(1);
    }

    BackupState &backupState = BackupState::getInstance();

    if (isPrimary)
    {
        std::cout << "Starting as primary" << std::endl;
        initializePrimary();
        // initializePrimary(true);
        return 0;
    }

    std::cout << "Starting as backup" << std::endl;
    std::string primaryServerAddress = argv[1];
    // std::string primaryServerAddress = "192.168.0.20";

    backupState.setPrimaryServerAddress(primaryServerAddress);

    while (backupState.getPrimaryServerAddress() != SELF_WIN_IP)
    {
        initializeBackup();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    initializePrimary(true);

    return 0;
}