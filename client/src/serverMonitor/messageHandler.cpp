#include "serverMonitor/messageHandler.hpp"

namespace serverMonitor
{

void MessageHandler::onStartSendFileMessage(const common::InitSendFile &initSendFile) const
{
    localMonitor::FileWatcher &fileWatcher = localMonitor::FileWatcher::getInstance(common::DEFAULT_CLIENT_SYNC_DIR);
    fileWatcher.pauseFileWatching();
}

void MessageHandler::onSendFileMessage(const common::InitSendFile &initSendFile) const
{
    localMonitor::FileWatcher &fileWatcher = localMonitor::FileWatcher::getInstance(common::DEFAULT_CLIENT_SYNC_DIR);
    fileWatcher.resumeFileWatching();
}

void MessageHandler::onDeleteFileMessage(const common::DeleteFile &deletedFile) const {};

} // namespace serverMonitor