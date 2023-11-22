#include "commandMessageHandler.hpp"

CommandMessageHandler::CommandMessageHandler(const TCPSocket &socket) : MessageHandler(socket)
{
}

CommandMessageHandler::CommandMessageHandler(const TCPSocket &socket, const std::string username)
    : MessageHandler(socket, username)
{
}

void CommandMessageHandler::handleJsonMessage(MessageHeader header, const std::string &message)
{
    switch (header.messageType)
    {
    case MessageType::LOGIN:
        std::cout << message << std::endl;
        break;

    default:
        std::cout << "Not implemented" << std::endl;
        break;
    }
}

void CommandMessageHandler::handleRawMessage(MessageHeader header)
{
    // auto bytes = socket.receive(header.dataSize);
    // std::string message(bytes.begin(), bytes.end());
    // std::cout << message << std::endl;
}