#include "localMonitor/localMonitor.hpp";

LocalMonitor::LocalMonitor(ServerMessageHandler &messageHandler, FileChangesQueue &changeQueue)
    : messageHandler(messageHandler), changeQueue(changeQueue)
{
}

void LocalMonitor::monitorChanges()
{
    while (true)
    {
        if (!changeQueue.empty())
        {
            auto change = changeQueue.pop();
            sendFileChange(change);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void LocalMonitor::sendFileChange(const common::FileChange &fileChange) const
{
    switch (fileChange.changeType)
    {
    case common::FileChangeType::FILE_CREATED: {
        common::File file(messageHandler.getSyncFolder() + fileChange.filename);
        messageHandler.sendInitSendFileMessage(file.getName(), file.getSize());
        messageHandler.sendFileMessage(file);
        messageHandler.receiveOK();
        break;
    }
    case common::FileChangeType::FILE_DELETED:
        messageHandler.sendDeleteFileMessage(fileChange.filename);
        break;
    default:
        break;
    }
};