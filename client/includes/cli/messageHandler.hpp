#pragma once

#include "clientMessageHandler.hpp"
#include <messages/message.hpp>
#include <models/listFiles.hpp>

namespace cli
{
class MessageHandler : public ClientMessageHandler
{
    using ClientMessageHandler::ClientMessageHandler;

  public:
    void sendListServerFilesMessage() const;
    void sendDownloadFileMessage(const std::string &filename) const;
    std::vector<common::FileInfo> receiveListFilesMessage() const;

  private:
    void onSendProgress(float progress) const override;
};
}