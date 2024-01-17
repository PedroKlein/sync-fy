#pragma once

#include "primary/connectionHandler.hpp"
#include "serverMessageHandler.hpp"
#include <filesystem/file.hpp>
#include <filesystem/fileChange.hpp>

namespace clientMonitor
{
/**
 * @class ClientMonitor
 * @brief Handles messages specific to the client monitor.
 *
 * This class is a subclass of ServerMessageHandler and provides
 * additional functionality for handling client monitor-specific messages.
 */
class MessageHandler : public ServerMessageHandler
{
    using ServerMessageHandler::ServerMessageHandler;

  private:
    /**
     * @brief Callback function called when a file send request is received.
     *
     * This function is called when a common::InitSendFile message is received
     * from the server. Subclasses can override this function to perform
     * custom actions when a file send request is received.
     *
     * @param initSendFile The common::InitSendFile message received.
     */
    void onSendFileMessage(const common::InitSendFile &initSendFile) const override;

    /**
     * @brief Callback function called when a file deletion request is received.
     *
     * This function is called when a common::DeleteFile message is received
     * from the server. Subclasses can override this function to perform
     * custom actions when a file deletion request is received.
     *
     * @param deletedFile The common::DeleteFile message received.
     */
    void onDeleteFileMessage(const common::DeleteFile &deletedFile) const override;
};
} // namespace clientMonitor