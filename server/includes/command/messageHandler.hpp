#pragma once

#include "connectionHandler.hpp"
#include "serverMessageHandler.hpp"
#include <filesystem/directory.hpp>
#include <filesystem/file.hpp>
#include <filesystem/fileChange.hpp>

namespace command
{
class MessageHandler : public ServerMessageHandler
{

    // implement contructor
  public:
    MessageHandler(common::TCPSocket &socket, std::string ip)
        : ServerMessageHandler(socket, ip), directory(common::Directory(getSyncFolder()))
    {
    }

  private:
    const common::Directory directory;

    void handleOtherMessage(const common::Message &message) const override;
    void handlePureHeaderMessage(const common::MessageHeader &message) const override;
    void onSendFileMessage(const common::InitSendFile &initSendFile) const override;
    void onDeleteFileMessage(const common::DeleteFile &deletedFile) const override;
};
} // namespace command