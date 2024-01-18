
#include "primary/userConnection.hpp"
#include "serverMessageHandler.hpp"
#include <filesystem/directory.hpp>
#include <memory>
#include <string>

class BackupMonitor
{
  public:
    BackupMonitor(ServerMessageHandler &messageHandler, std::shared_ptr<FileChangesQueue> changeQueue)
        : messageHandler(messageHandler), changeQueue(changeQueue), directory(messageHandler.getSyncFolder())
    {
    }

    void monitorChanges()
    {
        initialSync();

        isMonitoring = true;
        common::FileChange fileChange;

        do
        {
            if (changeQueue->tryPop(fileChange))
            {
                sendFileChange(fileChange);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        } while (isMonitoring && changeQueue.use_count() > 1);
    }

    void stopMonitoring()
    {
        isMonitoring = false;
    }

    void initialSync()
    {
        std::vector<common::FileInfo> dirFiles = directory.listFiles();

        for (auto &file : dirFiles)
        {
            common::FileChange fileChange;
            fileChange.changeType = common::FileChangeType::FILE_CREATED;
            fileChange.filename = file.filename;
            sendFileChange(fileChange);
        }
    }

    void sendFileChange(const common::FileChange &fileChange) const
    {
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

  private:
    ServerMessageHandler &messageHandler;
    std::shared_ptr<FileChangesQueue> changeQueue;
    const common::Directory directory;
    bool isMonitoring;

    void initialSync();

    void sendFileChange(const common::FileChange &fileChange) const;
};
