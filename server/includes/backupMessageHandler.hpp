#pragma once

#include <filesystem/file.hpp>
#include <messages/fileMessageHandler.hpp>
#include <messages/message.hpp>
#include <models/initBackupData.hpp>

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

  private:
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
};