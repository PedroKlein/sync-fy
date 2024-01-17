#include "primary/clientMonitor/messageHandler.hpp"

namespace clientMonitor
{
void MessageHandler::onSendFileMessage(const common::InitSendFile &initSendFile) const
{
    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
    UserConnection &userConnection = connectionHandler.getUserConnection(username);

    common::FileChange fileChange(initSendFile.filename, common::FileChangeType::FILE_CREATED);
    userConnection.addFileChange(ip, fileChange);
}

void MessageHandler::onDeleteFileMessage(const common::DeleteFile &deletedFile) const
{
    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
    UserConnection &userConnection = connectionHandler.getUserConnection(username);

    common::FileChange fileChange(deletedFile.filename, common::FileChangeType::FILE_DELETED);
    userConnection.addFileChange(ip, fileChange);
}
} // namespace clientMonitor