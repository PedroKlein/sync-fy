#pragma once

#include "connectionHandler.hpp"
#include "serverMessageHandler.hpp"
#include <filesystem/file.hpp>
#include <filesystem/fileChange.hpp>

namespace clientMonitor
{
class MessageHandler : public ServerMessageHandler
{
    using ServerMessageHandler::ServerMessageHandler;

  private:
    void onSendFileMessage(const common::InitSendFile &initSendFile) const override;
};
} // namespace clientMonitor