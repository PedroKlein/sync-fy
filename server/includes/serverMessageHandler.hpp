#pragma once

#include "models/login.hpp"
#include <filesystem/file.hpp>
#include <messages/fileMessageHandler.hpp>
#include <messages/message.hpp>

class ServerMessageHandler : public common::FileMessageHandler
{
  public:
    ServerMessageHandler(common::TCPSocket &socket, std::string ip) : common::FileMessageHandler(socket)
    {
        this->username = receiveLoginMessage();
        onLogin();
        this->syncFolder = common::DEFAULT_SERVER_SYNC_DIR + username + "/";

        this->ip = ip;

        std::cout << "Logged in as " << username << " from " << ip << std::endl;
    }

  protected:
    std::string ip;
    virtual void onLogin(){};

  private:
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

        return login.username;
    }
};