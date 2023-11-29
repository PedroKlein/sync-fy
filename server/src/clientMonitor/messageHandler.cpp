#include "clientMonitor/messageHandler.hpp"

namespace clientMonitor
{
// TODO: update files change list (update for the other client if needed)
void MessageHandler::onSendFileMessage(const common::InitSendFile &initSendFile) const
{
    std::cout << "Received init send file message" << std::endl;
}
} // namespace clientMonitor