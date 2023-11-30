#include "command/messageHandler.hpp"

namespace command
{
void MessageHandler::handleOtherMessage(const common::Message &message) const
{
}

void MessageHandler::handlePureHeaderMessage(const common::MessageHeader &header) const
{
    if (header.messageType == common::MessageType::INIT_LIST_FILES)
    {
        common::ListFiles listFiles;
        listFiles.files = directory.listFiles();
        sendModelMessage(listFiles);
    }
}

void MessageHandler::onSendFileMessage(const common::InitSendFile &initSendFile) const
{
    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
    UserConnection &userConnection = connectionHandler.getUserConnection(username);
    common::FileChange fileChange(initSendFile.filename, common::FileChangeType::FILE_CREATED);
    userConnection.addFileChange(fileChange);
}

void MessageHandler::onDeleteFileMessage(const common::DeleteFile &deletedFile) const
{
    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
    UserConnection &userConnection = connectionHandler.getUserConnection(username);
    common::FileChange fileChange(deletedFile.filename, common::FileChangeType::FILE_DELETED);
    userConnection.addFileChange(fileChange);
}
} // namespace command