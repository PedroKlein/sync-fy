#pragma once

#include <messages/messageHandler.hpp>

class CommandMessager : public MessageHandler
{
  public:
    CommandMessager(const TCPSocket &socket);
    CommandMessager(const TCPSocket &socket, const std::string username);

    void sendLoginMessage(const std::string username);

  protected:
    void handleJsonMessage(MessageHeader header, const std::string &message) override;
    void handleRawMessage(MessageHeader header) override;
};