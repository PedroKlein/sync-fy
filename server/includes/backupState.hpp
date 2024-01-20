#pragma once

#include <functional>
#include <models/connectedNodes.hpp>
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
        connectedClientIps.emplace_back(ip);
        if (updatedConnectedClientIpsCallback)
        {
            updatedConnectedClientIpsCallback();
        }
    }

    void addConnectedBackupNode(const common::Node &node)
    {
        connectedBackupNodes.emplace_back(node);
        if (updatedConnectedBackupNodesCallback)
        {
            updatedConnectedBackupNodesCallback();
        }
    }

    void removeConnectedClientIp(const std::string &ip)
    {
        connectedClientIps.erase(std::remove(connectedClientIps.begin(), connectedClientIps.end(), ip),
                                 connectedClientIps.end());
        if (updatedConnectedClientIpsCallback)
        {
            updatedConnectedClientIpsCallback();
        }
    }

    void removeConnectedBackupNode(const common::Node &node)
    {
        connectedBackupNodes.erase(std::remove(connectedBackupNodes.begin(), connectedBackupNodes.end(), node),
                                   connectedBackupNodes.end());
        if (updatedConnectedBackupNodesCallback)
        {
            updatedConnectedBackupNodesCallback();
        }
    }

    std::vector<common::Node> &getConnectedBackupNodes()
    {
        return connectedBackupNodes;
    }

    std::vector<std::string> &getConnectedIps()
    {
        return connectedClientIps;
    }

    int getServerId()
    {
        return serverId;
    }

    void setServerId(int id)
    {
        serverId = id;
    }

    std::string getPrimaryServerAddress()
    {
        return primaryServerAddress;
    }

    void setPrimaryServerAddress(const std::string &address)
    {
        primaryServerAddress = address;
    }

    void setConnectedClientIps(const std::vector<std::string> &ips)
    {
        connectedClientIps = ips;
    }

    void setConnectedBackupNodes(const std::vector<common::Node> &nodes)
    {
        connectedBackupNodes = nodes;
    }

  private:
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
