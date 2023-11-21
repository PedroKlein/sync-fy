#pragma once

#include <messages/messageHandler.hpp>

class CommandMessager : public MessageHandler
{
  public:
    CommandMessager(const TCPSocket &socket);

  protected:
    void handleJsonMessage(MessageHeader header) override;
    void handleRawMessage(MessageHeader header) override;
};