#pragma once

#include <filesystem/file.hpp>
#include <messages/message.hpp>
#include <messages/messageHandler.hpp>
#include <models/deleteFile.hpp>
#include <models/initReceiveFile.hpp>
#include <models/initSendFile.hpp>
#include <models/listFiles.hpp>

namespace localMonitor
{
class MessageHandler : public common::MessageHandler
{
    using common::MessageHandler::MessageHandler;

  public:
    void sendInitUploadFileMessage(const std::string &filename, size_t fileSize) const;
    void sendFileMessage(common::File &file) const;
    void sendDeleteFileMessage(const std::string &filename) const;

  protected:
    void handleMessage(const common::Message &message) override;
};
} // namespace localMonitor