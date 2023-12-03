#include "localMonitor/localMonitor.hpp"

namespace localMonitor
{
LocalMonitor::LocalMonitor(ServerMessageHandler &messageHandler, std::shared_ptr<FileChangesQueue> changeQueue)
    : messageHandler(messageHandler), changeQueue(changeQueue), directory(messageHandler.getSyncFolder())
{
}

void LocalMonitor::monitorChanges()
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

void LocalMonitor::stopMonitoring()
{
    isMonitoring = false;
}

void LocalMonitor::initialSync()
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

void LocalMonitor::sendFileChange(const common::FileChange &fileChange) const
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
} // namespace localMonitor