#include "cli/commandMessager.hpp"

CommandMessager::CommandMessager(const TCPSocket &socket) : MessageHandler(socket)
{
}

CommandMessager::CommandMessager(const TCPSocket &socket, const std::string username) : MessageHandler(socket, username)
{
}

void CommandMessager::handleJsonMessage(MessageHeader header, const std::string &message)
{
    std::cout << message << std::endl;
}

void CommandMessager::handleRawMessage(MessageHeader header)
{
    // auto bytes = socket.receive(header.dataSize);
    // std::string message(bytes.begin(), bytes.end());
    // std::cout << message << std::endl;
}