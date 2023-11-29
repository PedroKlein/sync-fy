#pragma once

#include "serverMessageHandler.hpp"
#include <filesystem/file.hpp>

namespace command
{
class MessageHandler : public ServerMessageHandler
{
    using ServerMessageHandler::ServerMessageHandler;

  public:
  private:
    void handleOtherMessage(const common::Message &message) const override;
    void onSendFileMessage(const common::InitSendFile &initSendFile) const override;
};
} // namespace command