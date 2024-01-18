#pragma once

#include "backupConnection.hpp"
#include "clientMonitor/messageHandler.hpp"
#include "command/messageHandler.hpp"
#include "localMonitor/localMonitor.hpp"
#include <memory>
#include <messages/messageHandler.hpp>
#include <socket/tcpSocket.hpp>
#include <thread>
#include <type_traits>

class ConnectionHandler
{
  public:
    void ConnectionHandler::onNetworkBackupSocketConnection(int clientSocketId, const std::string &ip)
    {
        std::thread([clientSocketId, ip]() {
            common::TCPSocket clientSocket(clientSocketId);
            // command::MessageHandler handler(clientSocket, ip);

            // ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
            // BackupConnection &backupConnection = connectionHandler.addBackupConnection(handler.getBackupname());
            // ClientConnection &clientConnection = backupConnection.addClientConnection(ip);

            // backupConnection.setCommandConnection(clientConnection, clientSocketId);

            // clientSocket.setOnDisconnect([&ip, &backupConnection, &connectionHandler, &handler]() {
            //     std::cout << "Command socket disconnected - " << handler.getBackupname() << ":" << ip << std::endl;
            //     handler.stopMonitoring();
            //     connectionHandler.removeBackupConnection(handler.getBackupname(), ip);
            // });

            // handler.monitorMessages();
        }).detach();
    }

    void ConnectionHandler::onBackupDataSocketConnection(int clientSocketId, const std::string &ip)
    {
    }

    ConnectionHandler &ConnectionHandler::getInstance()
    {
        static ConnectionHandler instance;
        return instance;
    }

    ConnectionHandler &ConnectionHandler::getInstance(int serverId)
    {
        static ConnectionHandler instance(serverId);
        return instance;
    }

    BackupConnection &addBackupConnection(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = backupConnections.find(ip);

        // backup connection already exists
        if (it != backupConnections.end())
        {
            return *(it->second);
        }

        std::unique_ptr<BackupConnection> backupConnection =
            std::make_unique<BackupConnection>(ip, serverId, std::make_shared<UserFileChangesQueue>());

        // Get a reference before moving the unique_ptr into the map
        BackupConnection &connectionRef = *backupConnection;

        backupConnections[ip] = std::move(backupConnection);

        return connectionRef;
    }

    BackupConnection &getBackupConnection(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = backupConnections.find(ip);
        if (it == backupConnections.end())
        {
            throw std::out_of_range("Backupname not found");
        }
        return *(it->second);
    }

    void removeBackupConnection(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = backupConnections.find(ip);

        if (it == backupConnections.end())
        {
            return;
        }

        backupConnections.erase(ip);
    }

    void addFileChange(const std::string &username, common::FileChange fileChange)
    {
        std::lock_guard<std::mutex> lock(mtx);

        for (auto &backupConnection : backupConnections)
        {
            backupConnections.second->fileChangesQueue->push(std::make_pair(username, fileChange));
        }
    }

    std::shared_ptr<UserFileChangesQueue> getFileChangesQueue(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = backupConnections.find(ip);
        if (it == backupConnections.end())
        {
            throw std::out_of_range("IP not found");
        }
        return (it->second->fileChangesQueue);
    }

  private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    ConnectionHandler() = default;

    ConnectionHandler(int serverID) : serverId(serverId);
    // ip -> backupConnection
    std::unordered_map<std::string, std::unique_ptr<BackupConnection>> backupConnections;
    std::mutex mtx;
    int serverId = 0;
};