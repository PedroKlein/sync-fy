#include "command/messageHandler.hpp"

namespace command
{
void MessageHandler::handleOtherMessage(const common::Message &message) const
{
    std::cout << "Received message: " << message << std::endl;
}

// TODO: update files change list (both clients if needed)
void MessageHandler::onSendFileMessage(const common::InitSendFile &initSendFile) const
{
    std::cout << "Received init send file message" << std::endl;
}
} // namespace command