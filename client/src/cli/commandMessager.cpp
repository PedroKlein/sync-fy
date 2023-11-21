#include "cli/commandMessager.hpp"

CommandMessager::CommandMessager(const TCPSocket &socket) : MessageHandler(socket)
{
}

void CommandMessager::handleJsonMessage(MessageHeader header)
{
    auto bytes = socket.receive(header.dataSize);
    std::string message(bytes.begin(), bytes.end());
    std::cout << message << std::endl;
}

void CommandMessager::handleRawMessage(MessageHeader header)
{
    auto bytes = socket.receive(header.dataSize);
    std::string message(bytes.begin(), bytes.end());
    std::cout << message << std::endl;
}