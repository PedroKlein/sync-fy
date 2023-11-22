#pragma once

#include <messages/messageHandler.hpp>

namespace cli
{
class MessageHandler : public common::MessageHandler
{
  public:
    MessageHandler(const common::TCPSocket &socket);
    MessageHandler(const common::TCPSocket &socket, const std::string username);

    void sendLoginMessage(const std::string username);

  protected:
    void handleJsonMessage(common::MessageHeader header, const std::string &message) override;
    void handleRawMessage(common::MessageHeader header) override;
};
}