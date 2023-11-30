#include "localMonitor/localMonitor.hpp"

namespace localMonitor
{
LocalMonitor::LocalMonitor(ServerMessageHandler &messageHandler, FileChangesQueue &changeQueue)
    : messageHandler(messageHandler), changeQueue(changeQueue), directory(messageHandler.getSyncFolder())
{
}

void LocalMonitor::monitorChanges()
{
    initialSync();

    common::FileChange fileChange;
    while (true)
    {
        if (changeQueue.tryPop(fileChange))
        {
            sendFileChange(fileChange);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
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
        common::File file(messageHandler.getSyncFolder() + fileChange.filename);
        messageHandler.sendFileMessage(file);
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