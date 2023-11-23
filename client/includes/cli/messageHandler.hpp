#pragma once

#include <messages/messageHandler.hpp>
#include <models/deleteFile.hpp>
#include <models/initRecieveFile.hpp>
#include <models/initSendFile.hpp>
#include <models/listFiles.hpp>

namespace cli
{
class MessageHandler : public common::MessageHandler
{
    using common::MessageHandler::MessageHandler;

    void sendUploadFileMessage(const std::string &filename) const;
    void sendDownloadFileMessage(const std::string &filename) const;
    void sendDeleteFileMessage(const std::string &filename) const;
    void sendListServerFilesMessage() const;
    void sendListClientFilesMessage() const;

  public:
  protected:
    void handleJsonMessage(common::MessageHeader header, const std::string &message) override;
    void handleRawMessage(common::MessageHeader header) override;
};
}