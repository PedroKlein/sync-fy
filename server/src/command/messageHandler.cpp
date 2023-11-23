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

    case common::HeaderType::RAW_DATA_HEADER: {
        std::cout << "RAW_DATA_HEADER" << std::endl;

        if (message.getMessageHeader().messageType == common::MessageType::SEND_RAW_DATA)
        {
            common::File::create("test.txt").receiveFile([&]() -> std::vector<char> {
                std::vector<char> chunk = message.getData();
                return chunk;
            });
            break;
        }
        break;
    }
    default:
        std::cout << "not implemented" << std::endl;
        break;
    }
}
} // namespace command