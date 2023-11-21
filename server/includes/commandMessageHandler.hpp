#pragma once

#include <messages/messageHandler.hpp>

class CommandMessageHandler : public MessageHandler
{
  public:
    CommandMessageHandler(const TCPSocket &socket);
    CommandMessageHandler(const TCPSocket &socket, const std::string username);

  protected:
    void handleJsonMessage(MessageHeader header) override;
    void handleRawMessage(MessageHeader header) override;
};