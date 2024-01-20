#pragma once

#include "backupConnection.hpp"
#include "backupDataMonitor.hpp"
#include <atomic/atomicVector.hpp>
#include <memory>
#include <messages/messageHandler.hpp>
#include <models/connectedIps.hpp>
#include <mutex>
#include <socket/tcpSocket.hpp>
#include <thread>
#include <type_traits>

class BackupConnectionHandler
{
  public:
    static void onNetworkBackupSocketConnection(int clientSocketId, const std::string &ip)
    {
        std::thread([clientSocketId, ip]() {
            // common::TCPSocket clientSocket(clientSocketId);

            // BackupConnectionHandler &connectionHandler = BackupConnectionHandler::getInstance();
            // BackupConnection &backupConnection = connectionHandler.addBackupConnection(ip);

            // BackupDataMonitor backupMonitor(clientSocket, connectionHandler.getFileChangesQueue(ip));

            // clientSocket.setOnDisconnect([&ip, &connectionHandler, &backupMonitor]() {
            //     std::cout << "Backup socket disconnected - " << ip << std::endl;
            //     backupMonitor.stopMonitoring();
            //     connectionHandler.removeBackupConnection(ip);
            // });

            // backupMonitor.monitorChanges();
        }).detach();
    }

    static void onBackupDataSocketConnection(int clientSocketId, const std::string &ip)
    {
        std::thread([clientSocketId, ip]() {
            common::TCPSocket clientSocket(clientSocketId);

            BackupConnectionHandler &connectionHandler = BackupConnectionHandler::getInstance();
            BackupConnection &backupConnection = connectionHandler.addBackupConnection(ip);

            BackupDataMonitor backupMonitor(clientSocket, connectionHandler.getFileChangesQueue(ip));

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

    void setServerId(int id)
    {
        serverId = id;
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

        if (it->second->connections > 0)
        {
            it->second->connections--;
            return;
        }

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

    void addConectedClientIp(const std::string &ip)
    {
        connectedClientIps.emplaceBack(ip);
    }

    void removeConectedClientIp(const std::string &ip)
    {
        connectedClientIps.remove(ip);
    }

    common::ConnectedIps getConnectedIps()
    {
        common::ConnectedIps connectedIps;
        connectedClientIps.forEach([&connectedIps](const std::string &ip) { connectedIps.ips.push_back(ip); });
        return connectedIps;
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
    BackupConnectionHandler() = default;

    // ip -> backupConnection
    std::unordered_map<std::string, std::unique_ptr<BackupConnection>> backupConnections;
    common::AtomicVector<std::string> connectedClientIps;
    std::mutex mtx;
    int serverId = 0;
};