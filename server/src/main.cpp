#include "primary/backupConnection/connectionHandler.hpp"
#include "primary/clientConnection/connectionHandler.hpp"
#include <backup/bullyElection.hpp>
#include <constants.hpp>
#include <cstdlib>
#include <iostream>
#include <socket/clientSocket.hpp>
#include <socket/serverSocket.hpp>
#include <unistd.h>

// for connecting with primary and receive updates on new nodes of network
// this is a client on the backup server
#define BACKUP_NETWORK_SOCKET_PORT 8769

// for connecting with primary and receive updates on new clients data
// this is a client on the backup server
#define BACKUP_DATA_SOCKET_PORT 8770

void initializePrimary()
{
    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();

    common::ServerSocket commandSocket(common::COMMAND_PORT);
    common::ServerSocket serverDataSocket(common::SERVER_DATA_PORT);
    common::ServerSocket clientDataSocket(common::CLIENT_DATA_PORT);

    // start listening inside a thread
    std::thread commandSocketThread(&common::ServerSocket::startListening, &commandSocket,
                                    ConnectionHandler::onCommandSocketConnection);
    std::thread serverDataSocketThread(&common::ServerSocket::startListening, &serverDataSocket,
                                       ConnectionHandler::onServerDataSocketConnection);
    std::thread clientDataSocketThread(&common::ServerSocket::startListening, &clientDataSocket,
                                       ConnectionHandler::onClientDataSocketConnection);

    // TODO: For primary server
    // 1. Listen for backup server connections

    // 2. Manage backups, connections and disconnections (keep a list of them and update them with their pairs to cloase
    // the ring for the election) keeps pinging them with a ALIVE message

    // 3. After a change on a client data, send the new data to the backup servers (keep a atomic fifo queue of the
    // changes)

    commandSocketThread.join();
    serverDataSocketThread.join();
    clientDataSocketThread.join();
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
    common::ClientSocket networkSocket(primaryServerAddress, BACKUP_NETWORK_SOCKET_PORT);

    // 2. Receive data from primary server of next backup and data from clients

    // 3. If primary fails, run bully election

    // 4. If elected, run as primary (initializePrimary) and connect to all clients

    // 5. If not elected, repeat from step 1

    common::ClientSocket recoverySocket("localhost", common::CLIENT_RECOVERY_PORT);

    return 0;
}