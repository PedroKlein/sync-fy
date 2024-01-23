#pragma once
#include "backupConnection.hpp"
#include "backupMonitor.hpp"
#include "backupState.hpp"
#include <memory>
#include <messages/messageHandler.hpp>
#include <models/connectedIps.hpp>
#include <models/connectedNodes.hpp>
#include <mutex>
#include <socket/tcpSocket.hpp>
#include <thread>
#include <type_traits>

class BackupConnectionHandler
{
  public:
    static void onBackupSocketConnection(int clientSocketId, const std::string &ip)
    {
        std::thread([clientSocketId, ip]() {
            common::TCPSocket clientSocket(clientSocketId);

            BackupConnectionHandler &connectionHandler = BackupConnectionHandler::getInstance();
            BackupConnection &backupConnection = connectionHandler.addBackupConnection(ip);

            BackupMessageHandler messageHandler(clientSocket);
            messageHandler.sendServerIdMessage(backupConnection.id);

            BackupMonitor backupMonitor(clientSocket, connectionHandler.getFileChangesQueue(ip),
                                        connectionHandler.getClientAndNodeChanges(ip));

            clientSocket.setOnDisconnect([&ip, &connectionHandler, &backupMonitor]() {
                std::cout << "Backup socket disconnected - " << ip << std::endl;
                backupMonitor.stopMonitoring();
                connectionHandler.removeBackupConnection(ip);
            });

            backupMonitor.monitorChanges();
        }).detach();
    }

    static BackupConnectionHandler &getInstance()
    {
        static BackupConnectionHandler instance;
        return instance;
    }

    BackupConnection &addBackupConnection(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = backupConnections.find(ip);

        // backup connection already exists
        if (it != backupConnections.end())
        {
            it->second->connections++;
            return *(it->second);
        }

        BackupState &backupState = BackupState::getInstance();

        std::unique_ptr<BackupConnection> backupConnection =
            std::make_unique<BackupConnection>(ip, backupState.getServerId(), std::make_shared<UserFileChangesQueue>(),
                                               std::make_shared<HasClientAndNodeIpsChange>(true, true));

        // Get a reference before moving the unique_ptr into the map
        BackupConnection &connectionRef = *backupConnection;

        backupConnections[ip] = std::move(backupConnection);

        backupState.addConnectedBackupNode(common::Node(ip, connectionRef.id));

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

        if (it->second->connections > 0)
        {
            it->second->connections--;
            return;
        }

        BackupState &backupState = BackupState::getInstance();
        backupState.removeConnectedBackupNode(common::Node(ip, it->second->id));

        backupConnections.erase(ip);
    }

    void addFileChange(const std::string &username, common::FileChange fileChange)
    {
        std::lock_guard<std::mutex> lock(mtx);

        for (auto &backupConnection : backupConnections)
        {
            backupConnection.second->fileChangesQueue->push(std::make_pair(username, fileChange));
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

    std::shared_ptr<HasClientAndNodeIpsChange> getClientAndNodeChanges(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = backupConnections.find(ip);
        if (it == backupConnections.end())
        {
            throw std::out_of_range("IP not found");
        }
        return (it->second->clientAndNodeChanges);
    }

    void hasUpdatedConnectedClientIps()
    {
        // std::lock_guard<std::mutex> lock(mtx);
        for (auto &connections : backupConnections)
        {
            connections.second->clientAndNodeChanges->first = true;
        }
    }

    void hasUpdatedConnectedBackupNodes()
    {
        // std::lock_guard<std::mutex> lock(mtx);
        for (auto &connections : backupConnections)
        {
            connections.second->clientAndNodeChanges->second = true;
        }
    }

  private:
    // ip -> backupConnection
    std::unordered_map<std::string, std::unique_ptr<BackupConnection>> backupConnections;
    std::mutex mtx;

    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    BackupConnectionHandler() = default;
};