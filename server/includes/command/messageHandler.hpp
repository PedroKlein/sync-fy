#pragma once

#include "connectionHandler.hpp"
#include "serverMessageHandler.hpp"
#include <filesystem/file.hpp>
#include <filesystem/fileChange.hpp>

namespace command
{
class MessageHandler : public ServerMessageHandler
{
    using ServerMessageHandler::ServerMessageHandler;

  private:
    void handleOtherMessage(const common::Message &message) const override;
    void onSendFileMessage(const common::InitSendFile &initSendFile) const override;
    void onDeleteFileMessage(const common::DeleteFile &deletedFile) const override;
};
} // namespace command