#pragma once

#include <filesystem/file.hpp>
#include <messages/fileMessageHandler.hpp>
#include <messages/message.hpp>
#include <models/initBackupData.hpp>

class BackupMessageHandler : public common::FileMessageHandler
{
  public:
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
};