#pragma once

#include <filesystem/file.hpp>
#include <messages/fileMessageHandler.hpp>
#include <messages/message.hpp>
#include <models/connectedIps.hpp>
#include <models/connectedNodes.hpp>
#include <models/initBackupData.hpp>

// callbacks types for clientConnectedIps and connectedNodes
using ConnectedIpsCallback = std::function<void(const common::ConnectedIps &)>;
using ConnectedNodesCallback = std::function<void(const common::ConnectedNodes &)>;

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

    void sendConnectedIpsMessage(const common::ConnectedIps &connectedIps) const
    {
        sendModelMessage(connectedIps);
    }

    void sendConnectedNodesMessage(const common::ConnectedNodes &connectedNodes) const
    {
        sendModelMessage(connectedNodes);
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
        default:
            throw std::runtime_error("Invalid message type");
        }
    };

    void handleInitBackupDataMessage(const common::Message &message)
    {
        std::string data(message.getData().begin(), message.getData().end());

        common::InitBackupData initBackupData;
        initBackupData.fromJson(data);

        setUsername(initBackupData.username);
    };

    void handleConnectedIpsMessage(const common::Message &message)
    {
        std::string data(message.getData().begin(), message.getData().end());

        common::ConnectedIps connectedIps;
        connectedIps.fromJson(data);

        if (connectedIpsCallback)
        {
            connectedIpsCallback(connectedIps);
        }
    };

    void handleConnectedNodesMessage(const common::Message &message)
    {
        std::string data(message.getData().begin(), message.getData().end());

        common::ConnectedNodes connectedNodes;
        connectedNodes.fromJson(data);

        if (connectedNodesCallback)
        {
            connectedNodesCallback(connectedNodes);
        }
    };
};