#pragma once

#include "serverMessageHandler.hpp"
#include <filesystem/file.hpp>
#include <messages/message.hpp>

namespace clientMonitor
{
class MessageHandler : public ServerMessageHandler
{
    using common::MessageHandler::MessageHandler;

  private:
    void onSendFileMessage(const common::InitSendFile &initSendFile) override;
};
} // namespace clientMonitor