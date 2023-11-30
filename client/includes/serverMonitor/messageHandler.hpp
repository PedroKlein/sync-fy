#pragma once

#include "clientMessageHandler.hpp"
#include "localMonitor/fileWatcher.hpp"
#include <filesystem/file.hpp>
#include <filesystem/fileChange.hpp>

namespace serverMonitor
{
class MessageHandler : public ClientMessageHandler
{
    using ClientMessageHandler::ClientMessageHandler;

  private:
    void onSendFileMessage(const common::InitSendFile &initSendFile) const override;
    void onStartSendFileMessage(const common::InitSendFile &initSendFile) const override;
    void onDeleteFileMessage(const common::DeleteFile &deletedFile) const override;
};
} // namespace serverMonitor