#include "cli/messageHandler.hpp"

namespace cli
{
MessageHandler::MessageHandler(const common::TCPSocket &socket) : common::MessageHandler(socket)
{
}

MessageHandler::MessageHandler(const common::TCPSocket &socket, const std::string username)
    : common::MessageHandler(socket, username)
{
}

void MessageHandler::handleJsonMessage(common::MessageHeader header, const std::string &message)
{
    std::cout << message << std::endl;
}

void MessageHandler::handleRawMessage(common::MessageHeader header)
{
    // auto bytes = socket.receive(header.dataSize);
    // std::string message(bytes.begin(), bytes.end());
    // std::cout << message << std::endl;
}
} // namespace cli