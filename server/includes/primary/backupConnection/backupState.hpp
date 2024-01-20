#pragma once

#include <atomic/atomicVector.hpp>
#include <functional>
#include <models/connectedIps.hpp>
#include <models/connectedNodes.hpp>

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
        connectedClientIps.emplaceBack(ip);
        if (updatedConnectedClientIpsCallback)
        {
            updatedConnectedClientIpsCallback();
        }
    }

    void removeConectedClientIp(const std::string &ip)
    {
        connectedClientIps.remove(ip);
        if (updatedConnectedClientIpsCallback)
        {
            updatedConnectedClientIpsCallback();
        }
    }

    common::ConnectedIps getConnectedIps()
    {
        common::ConnectedIps connectedIps;
        connectedClientIps.forEach([&connectedIps](const std::string &ip) { connectedIps.ips.push_back(ip); });
        return connectedIps;
    }

    void addConnectedBackupNode(const common::Node &node)
    {
        connectedBackupNodes.emplaceBack(node);
        if (updatedConnectedBackupNodesCallback)
        {
            updatedConnectedBackupNodesCallback();
        }
    }

    void removeConnectedBackupNode(const common::Node &node)
    {
        connectedBackupNodes.remove(node);
        if (updatedConnectedBackupNodesCallback)
        {
            updatedConnectedBackupNodesCallback();
        }
    }

    common::ConnectedNodes getConnectedBackupNodes()
    {
        common::ConnectedNodes connectedNodes;
        connectedBackupNodes.forEach(
            [&connectedNodes](const common::Node &node) { connectedNodes.nodes.push_back(node); });
        return connectedNodes;
    }

  private:
    common::AtomicVector<std::string> connectedClientIps;
    common::AtomicVector<common::Node> connectedBackupNodes;
    Callback updatedConnectedClientIpsCallback;
    Callback updatedConnectedBackupNodesCallback;

    BackupState()
    {
    }

    BackupState(const BackupState &) = delete;
    BackupState &operator=(const BackupState &) = delete;
};
