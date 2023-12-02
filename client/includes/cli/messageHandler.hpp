#pragma once

#include "clientMessageHandler.hpp"
#include <messages/message.hpp>
#include <models/listFiles.hpp>

namespace cli
{
/**
 * @class MessageHandler
 * @brief Handles messages specific to the client application.
 */
class MessageHandler : public ClientMessageHandler
{
    using ClientMessageHandler::ClientMessageHandler;

  public:
    /**
     * @brief Sends a message to request the list of files available on the server.
     */
    void sendListServerFilesMessage() const;

    /**
     * @brief Sends a message to request the download of a specific file from the server.
     * @param filename The name of the file to download.
     */
    void sendDownloadFileMessage(const std::string &filename) const;

    /**
     * @brief Receives a message containing the list of files from the server.
     * @return A vector of FileInfo objects representing the server's files.
     */
    std::vector<common::FileInfo> receiveListFilesMessage() const;

  private:
    /**
     * @brief Callback function invoked during file sending to report progress.
     * @param progress The progress of the file transfer (0.0 to 1.0).
     */
    void onSendProgress(float progress) const override;
};
}