#pragma once

#include "backupState.hpp"
#include <filesystem/file.hpp>
#include <messages/fileMessageHandler.hpp>
#include <messages/message.hpp>
#include <models/connectedIps.hpp>
#include <models/connectedNodes.hpp>
#include <models/initBackupData.hpp>
#include <models/serverId.hpp>

// callbacks types for clientConnectedIps and connectedNodes
using ConnectedIpsCallback = std::function<void(const std::vector<std::string> &)>;
using ConnectedNodesCallback = std::function<void(const std::vector<common::Node> &)>;

class BackupMessageHandler : public common::FileMessageHandler
{
  public:
    BackupMessageHandler(common::TCPSocket &socket) : common::FileMessageHandler(socket)
    {
    }

    BackupMessageHandler(common::TCPSocket &socket, std::string username) : common::FileMessageHandler(socket)
    {
        this->username = username;
        this->syncFolder = common::DEFAULT_SERVER_SYNC_DIR + username + "/";

        try
        {
            std::filesystem::create_directory(this->syncFolder);
        }
        catch (const std::filesystem::filesystem_error &ex)
        {
            std::cerr << "Error creating directory: " << ex.what() << std::endl;
        }
    }

    void sendInitBackupDataMessage(const std::string &username) const
    {
        common::InitBackupData initBackupData(username);
        sendModelMessage(initBackupData);
    }

    void sendConnectedIpsMessage(const std::vector<std::string> &connectedIps) const
    {
        common::ConnectedIps ips(connectedIps);
        sendModelMessage(ips);
    }

    void sendConnectedNodesMessage(const std::vector<common::Node> &connectedNodes) const
    {
        common::ConnectedNodes nodes(connectedNodes);
        sendModelMessage(nodes);
    }

    void sendServerIdMessage(int id) const
    {
        common::ServerId serverId(id);
        sendModelMessage(serverId);
    }

    void setConnectedIpsCallback(ConnectedIpsCallback connectedIpsCallback)
    {
        this->connectedIpsCallback = connectedIpsCallback;
    }

    void setConnectedNodesCallback(ConnectedNodesCallback connectedNodesCallback)
    {
        this->connectedNodesCallback = connectedNodesCallback;
    }

  private:
    ConnectedIpsCallback connectedIpsCallback;
    ConnectedNodesCallback connectedNodesCallback;

    void setUsername(std::string username)
    {
        this->username = username;
        this->syncFolder = common::DEFAULT_SERVER_SYNC_DIR + username + "/";

        try
        {
            std::filesystem::create_directory(this->syncFolder);
        }
        catch (const std::filesystem::filesystem_error &ex)
        {
            std::cerr << "Error creating directory: " << ex.what() << std::endl;
        }
    }

    void handleOtherMessage(const common::Message &message) override
    {
        switch (message.getMessageHeader().messageType)
        {
        case common::MessageType::INIT_BACKUP_DATA:
            handleInitBackupDataMessage(message);
            break;
        case common::MessageType::CONNECTED_IPS:
            handleConnectedIpsMessage(message);
            break;
        case common::MessageType::CONNECTED_NODES:
            handleConnectedNodesMessage(message);
            break;
        case common::MessageType::SERVER_ID:
            handleServerIdMessage(message);
            break;
        case common::MessageType::ALIVE:
            break;
        default:
            throw std::runtime_error("Invalid message type");
        }
    };

    void handleInitBackupDataMessage(const common::Message &message)
    {
        auto raw = message.getData();
        std::string data(raw.begin(), raw.end());

        common::InitBackupData initBackupData;
        initBackupData.fromJson(data);

        setUsername(initBackupData.username);
    };

    void handleConnectedIpsMessage(const common::Message &message)
    {
        auto raw = message.getData();
        std::string data(raw.begin(), raw.end());

        common::ConnectedIps connectedIps;
        connectedIps.fromJson(data);

        if (connectedIpsCallback)
        {
            connectedIpsCallback(connectedIps.ips);
        }
    };

    void handleConnectedNodesMessage(const common::Message &message)
    {
        auto raw = message.getData();
        std::string data(raw.begin(), raw.end());

        common::ConnectedNodes connectedNodes;
        connectedNodes.fromJson(data);

        if (connectedNodesCallback)
        {
            connectedNodesCallback(connectedNodes.nodes);
        }
    };

    void handleServerIdMessage(const common::Message &message)
    {
        auto raw = message.getData();
        std::string data(raw.begin(), raw.end());

        common::ServerId serverId;
        serverId.fromJson(data);

        // separation of concerns was long dead in this codebase before this atrocity
        BackupState &backupState = BackupState::getInstance();

        backupState.setServerId(serverId.serverId);
    };
};