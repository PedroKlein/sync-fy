#include "clientMonitor/messageHandler.hpp"

namespace clientMonitor
{
void MessageHandler::handleMessage(const common::Message &message)
{
    const auto header = message.getMessageHeader();

    switch (header.headerType)
    {
    case common::HeaderType::JSON_HEADER: {
        std::cout << "JSON_HEADER" << std::endl;
        std::string data(message.getData().begin(), message.getData().end());
        std::cout << data << std::endl;
        break;
    }

    default:
        std::cout << "not implemented" << std::endl;
        break;
    }
}
} // namespace clientMonitor