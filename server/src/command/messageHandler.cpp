#include "command/messageHandler.hpp"

namespace command
{
void MessageHandler::handleMessage(const common::Message &message)
{
    switch (message.getMessageHeader().headerType)
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
} // namespace command