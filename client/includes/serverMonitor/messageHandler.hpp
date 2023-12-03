#pragma once

#include "clientMessageHandler.hpp"
#include "localMonitor/fileWatcher.hpp"
#include <filesystem/file.hpp>
#include <filesystem/fileChange.hpp>

namespace serverMonitor
{
/**
 * @class MessageHandler
 * @brief Handles server-specific messages derived from ClientMessageHandler.
 * 
 * This class extends ClientMessageHandler to provide additional handling
 * for server-specific messages.
 */
class MessageHandler : public ClientMessageHandler
{
    using ClientMessageHandler::ClientMessageHandler;

  private:
    /**
     * @brief Handles the reception of a "Send File" message.
     * @param initSendFile The initiation message for sending a file.
     */
    void onSendFileMessage(const common::InitSendFile &initSendFile) const override;

    /**
     * @brief Handles the reception of a "Start Send File" message.
     * @param initSendFile The initiation message for starting to send a file.
     */
    void onStartSendFileMessage(const common::InitSendFile &initSendFile) const override;

    /**
     * @brief Handles the reception of a "Delete File" message.
     * @param deletedFile The message containing information about the deleted file.
     */
    void onDeleteFileMessage(const common::DeleteFile &deletedFile) const override;
};
} // namespace serverMonitor