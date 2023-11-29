#pragma once

#include "models/login.hpp"
#include <filesystem/file.hpp>
#include <messages/fileMessageHandler.hpp>
#include <messages/message.hpp>

class ServerMessageHandler : public common::FileMessageHandler
{
  public:
    ServerMessageHandler(const common::TCPSocket &socket) : common::FileMessageHandler(socket, username, syncFolder)
    {
        username = receiveLoginMessage();
        onLogin();
        syncFolder = common::DEFAULT_SERVER_SYNC_DIR + username + "/";

        common::FileMessageHandler(socket, username, syncFolder);

        std::cout << "Logged in as " << username << std::endl;
    }

  protected:
    virtual void onLogin(){};

  private:
    // TODO: maybe add a reference to the files change list here?
    // since this has the username and we have one list of files change per user

    std::string receiveLoginMessage()
    {
        common::MessageHeader header = receiveHeader();

        if (header.messageType != common::MessageType::LOGIN)
        {
            throw std::runtime_error("Expected login message");
        }

        std::vector<char> messageData(header.dataSize);
        socket.receive(messageData.data(), header.dataSize);
        std::string message(messageData.begin(), messageData.end());

        common::Login login;
        login.fromJson(message);

        sendOK();

        return login.username;
    }
};