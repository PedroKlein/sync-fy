
#include "backupMessageHandler.hpp"
#include "primary/backupConnection/backupConnection.hpp"
#include <filesystem/directory.hpp>
#include <memory>
#include <string>
#include <thread>

class BackupDataMonitor
{
  public:
    BackupDataMonitor(common::TCPSocket &socket, std::shared_ptr<UserFileChangesQueue> changeQueue)
        : changeQueue(changeQueue), socket(socket)
    {
    }

    void monitorChanges()
    {
        initialSync();

        isMonitoring = true;
        UserFileChange userFileChange;

        do
        {
            while (changeQueue->tryPop(userFileChange))
            {
                sendFileChange(userFileChange.second, userFileChange.first);
            }
            std::this_thread::sleep_for(std::chrono::microseconds(200));
        } while (isMonitoring && changeQueue.use_count() > 1);
    }

    void stopMonitoring()
    {
        isMonitoring = false;
    }

  private:
    common::TCPSocket &socket;
    std::shared_ptr<UserFileChangesQueue> changeQueue;
    bool isMonitoring;

    void initialSync()
    {
        for (const auto &entry : std::filesystem::directory_iterator(common::DEFAULT_SERVER_SYNC_DIR))
        {
            if (entry.is_directory())
            {
                common::Directory directory(entry.path().string());
                std::vector<common::FileInfo> dirFiles = directory.listFiles();

                std::string username = directory.getDirectoryName();

                for (auto &file : dirFiles)
                {
                    common::FileChange fileChange;
                    fileChange.changeType = common::FileChangeType::FILE_CREATED;
                    fileChange.filename = file.filename;
                    sendFileChange(fileChange, username);
                }
            }
        }
    }

    void sendFileChange(const common::FileChange &fileChange, std::string username) const
    {
        BackupMessageHandler messageHandler(socket, username);

        messageHandler.sendInitBackupDataMessage(username);

        switch (fileChange.changeType)
        {
        case common::FileChangeType::FILE_CREATED: {
            try
            {
                common::File file(messageHandler.getSyncFolder() + fileChange.filename);
                messageHandler.sendFileMessage(file);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }

            break;
        }
        case common::FileChangeType::FILE_DELETED:
            messageHandler.sendDeleteFileMessage(fileChange.filename);
            break;
        default:
            break;
        }
    };
};
