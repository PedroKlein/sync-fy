#include "clientMonitor/messageHandler.hpp"

namespace clientMonitor
{
void MessageHandler::onSendFileMessage(const common::InitSendFile &initSendFile) const
{
    ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
    UserConnection &userConnection = connectionHandler.getUserConnection(username);

    common::FileChange fileChange(initSendFile.filename, common::FileChangeType::FILE_CREATED);
    userConnection.addFileChange(ip, fileChange);

    std::cout << "Received init send file message" << std::endl;
}
} // namespace clientMonitor