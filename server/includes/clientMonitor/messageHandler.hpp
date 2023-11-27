#pragma once

#include <filesystem/file.hpp>
#include <messages/message.hpp>
#include <messages/messageHandler.hpp>

namespace clientMonitor
{
class MessageHandler : public common::MessageHandler
{
    using common::MessageHandler::MessageHandler;

  public:
  protected:
    void handleMessage(const common::Message &message) override;
};
} // namespace clientMonitor