#include "primary/clientMonitor/messageHandler.hpp"

namespace clientMonitor
{
void MessageHandler::onSendFileMessage(const common::InitSendFile &initSendFile) const
{
    ClientConnectionHandler &connectionHandler = ClientConnectionHandler::getInstance();
    UserConnection &userConnection = connectionHandler.getUserConnection(username);

    common::FileChange fileChange(initSendFile.filename, common::FileChangeType::FILE_CREATED);
    userConnection.addFileChange(ip, fileChange);

    BackupConnectionHandler &backupConnectionHandler = BackupConnectionHandler::getInstance();
    backupConnectionHandler.addFileChange(username, fileChange);
}

void MessageHandler::onDeleteFileMessage(const common::DeleteFile &deletedFile) const
{
    ClientConnectionHandler &connectionHandler = ClientConnectionHandler::getInstance();
    UserConnection &userConnection = connectionHandler.getUserConnection(username);

    common::FileChange fileChange(deletedFile.filename, common::FileChangeType::FILE_DELETED);
    userConnection.addFileChange(ip, fileChange);

    BackupConnectionHandler &backupConnectionHandler = BackupConnectionHandler::getInstance();
    backupConnectionHandler.addFileChange(username, fileChange);
}
} // namespace clientMonitor