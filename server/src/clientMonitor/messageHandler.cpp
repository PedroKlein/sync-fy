#include "clientMonitor/messageHandler.hpp"

namespace clientMonitor
{
// TODO: update files change list (update for the other client if needed)
void MessageHandler::onSendFileMessage(const common::InitSendFile &initSendFile)
{
    std::cout << "Received init send file message: " << initSendFile << std::endl;
}
} // namespace clientMonitor