#pragma once

#include <constants.hpp>
#include <filesystem/file.hpp>
#include <messages/fileMessageHandler.hpp>
#include <messages/message.hpp>
#include <models/deleteFile.hpp>
#include <models/initReceiveFile.hpp>
#include <models/initSendFile.hpp>
#include <models/listFiles.hpp>
#include <models/login.hpp>
#include <socket/tcpSocket.hpp>

class ClientMessageHandler : public common::FileMessageHandler
{
  public:
    ClientMessageHandler(const common::TCPSocket &socket, std::string username)
        : common::FileMessageHandler(socket, username, common::DEFAULT_CLIENT_SYNC_DIR)
    {
        sendLoginMessage(username);
        onLogin();
    }

  protected:
    virtual void onLogin(){};

  private:
    void sendLoginMessage(const std::string &username)
    {
        common::Login login(username);
        sendModelMessage(login);
    }
};