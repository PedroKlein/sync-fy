#include "command/messageHandler.hpp"

namespace command
{
void MessageHandler::handleOtherMessage(const common::Message &message) const
{
    std::cout << "Received message: " << message << std::endl;
}

void MessageHandler::onSendFileMessage(const common::InitSendFile &initSendFile) const
{
    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
    UserConnection &userConnection = connectionHandler.getUserConnection(username);
    common::FileChange fileChange(initSendFile.filename, common::FileChangeType::FILE_CREATED);
    userConnection.addFileChange(fileChange);
    std::cout << "Received init send file message" << std::endl;
}
} // namespace command