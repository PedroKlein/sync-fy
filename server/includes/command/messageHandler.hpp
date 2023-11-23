#pragma once

#include <messages/messageHandler.hpp>

namespace command
{
class MessageHandler : public common::MessageHandler
{
    using common::MessageHandler::MessageHandler;

  public:
  protected:
    void handleJsonMessage(common::MessageHeader header, const std::string &message) override;
    void handleRawMessage(common::MessageHeader header) override;
};
} // namespace command