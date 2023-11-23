#pragma once

#include <messages/message.hpp>
#include <messages/messageHandler.hpp>

namespace command
{
class MessageHandler : public common::MessageHandler
{
    using common::MessageHandler::MessageHandler;

  public:
  protected:
    void handleMessage(const common::Message &message) override;
};
} // namespace command