#pragma once

#include <functional>
#include <models/connectedNodes.hpp>
#include <mutex>
#include <string>
#include <vector>

// TODO: make it thread safe
class BackupState
{
  public:
    using Callback = std::function<void()>;

    static BackupState &getInstance()
    {
        static BackupState instance;
        return instance;
    }

    void setUpdatedConnectedClientIpsCallback(Callback callback)
    {
        updatedConnectedClientIpsCallback = callback;
    }

    void setUpdatedConnectedBackupNodesCallback(Callback callback)
    {
        updatedConnectedBackupNodesCallback = callback;
    }

    void addConectedClientIp(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        connectedClientIps.emplace_back(ip);
        if (updatedConnectedClientIpsCallback)
        {
            updatedConnectedClientIpsCallback();
        }
    }

    void addConnectedBackupNode(const common::Node &node)
    {
        std::lock_guard<std::mutex> lock(mtx);
        connectedBackupNodes.emplace_back(node);
        if (updatedConnectedBackupNodesCallback)
        {
            updatedConnectedBackupNodesCallback();
        }
    }

    void removeConnectedClientIp(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        connectedClientIps.erase(std::remove(connectedClientIps.begin(), connectedClientIps.end(), ip),
                                 connectedClientIps.end());
        if (updatedConnectedClientIpsCallback)
        {
            updatedConnectedClientIpsCallback();
        }
    }

    void removeConnectedBackupNode(const common::Node &node)
    {
        std::lock_guard<std::mutex> lock(mtx);
        connectedBackupNodes.erase(std::remove(connectedBackupNodes.begin(), connectedBackupNodes.end(), node),
                                   connectedBackupNodes.end());
        if (updatedConnectedBackupNodesCallback)
        {
            updatedConnectedBackupNodesCallback();
        }
    }

    std::vector<common::Node> &getConnectedBackupNodes()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return connectedBackupNodes;
    }

    std::vector<std::string> &getConnectedIps()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return connectedClientIps;
    }

    int getServerId()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return serverId;
    }

    void setServerId(int id)
    {
        std::lock_guard<std::mutex> lock(mtx);
        serverId = id;
    }

    std::string getPrimaryServerAddress()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return primaryServerAddress;
    }

    void setPrimaryServerAddress(const std::string &address)
    {
        std::lock_guard<std::mutex> lock(mtx);
        primaryServerAddress = address;
    }

    void setConnectedClientIps(const std::vector<std::string> &ips)
    {
        std::lock_guard<std::mutex> lock(mtx);
        connectedClientIps = ips;
    }

    void setConnectedBackupNodes(const std::vector<common::Node> &nodes)
    {
        std::lock_guard<std::mutex> lock(mtx);
        connectedBackupNodes = nodes;
    }

  private:
    std::mutex mtx;
    int serverId = 0;
    std::string primaryServerAddress = "localhost";
    std::vector<std::string> connectedClientIps;
    std::vector<common::Node> connectedBackupNodes;

    Callback updatedConnectedClientIpsCallback;
    Callback updatedConnectedBackupNodesCallback;

    BackupState()
    {
    }

    BackupState(const BackupState &) = delete;
    BackupState &operator=(const BackupState &) = delete;
};
