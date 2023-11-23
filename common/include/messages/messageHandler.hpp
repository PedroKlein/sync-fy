#pragma once

#include "message.hpp"
#include "messageTypes.hpp"
#include "models/baseModel.hpp"
#include "models/login.hpp"
#include "socket/tcpSocket.hpp"
#include <iostream>

namespace common
{
class MessageHandler
{
  public:
    MessageHandler(const TCPSocket &socket) : socket(socket)
    {
        recieveLoginMessage();
    }

    MessageHandler(const TCPSocket &socket, const std::string username) : socket(socket), username(username)
    {
        sendLoginMessage(username);
    }

    void sendModelMessage(const BaseModel &model) const
    {
        Message message(model.getType(), model.toJson());
        sendMessage(message);
    }

    void sendOK() const
    {
        Message okMessage(common::MessageType::OK);
        sendMessage(okMessage, false);
    }

    void sendExit() const
    {
        Message exitMessage(common::MessageType::OK);
        sendMessage(exitMessage);
    }

    void sendRawMessage(const std::vector<char> &data) const
    {
        Message message(common::MessageType::SEND_RAW, data);
        sendMessage(message);
    }

    // TODO: This is more confusing then it needs to be, try to simplify it
    void receiveMessage()
    {
        MessageHeader header = receiveHeader();

        if (header.headerType == HeaderType::PURE_HEADER)
        {
            Message message(header.messageType);
            sendOK();
            handleMessage(message);
            return;
        }

        std::vector<char> messageData(header.dataSize);
        socket.receive(messageData.data(), header.dataSize);
        sendOK();

        if (header.headerType == HeaderType::JSON_HEADER)
        {
            Message message(header.messageType, std::string(messageData.begin(), messageData.end()));
            handleMessage(message);
            return;
        }

        Message message(header.messageType, messageData);

        handleMessage(message);
    }

    void monitorMessages()
    {
        while (true)
        {
            receiveMessage();
        }
    }

    std::string getUsername() const
    {
        return username;
    }

  protected:
    const TCPSocket &socket;
    std::string username;

    virtual void handleMessage(const Message &message) = 0;

  private:
    void sendMessage(const Message &message, bool waitForResponse = true) const
    {
        std::vector<char> serialized = message.serialize();
        socket.send(serialized.data(), serialized.size());

        if (waitForResponse)
        {
            recieveOK();
        }
    }

    void sendLoginMessage(const std::string username)
    {
        common::Login login(username);
        sendModelMessage(login);
    }

    void recieveLoginMessage()
    {
        MessageHeader header = receiveHeader();

        if (header.messageType != MessageType::LOGIN)
        {
            throw std::runtime_error("Expected login message");
        }

        std::vector<char> messageData(header.dataSize);
        socket.receive(messageData.data(), header.dataSize);
        std::string message(messageData.begin(), messageData.end());

        Login login;
        login.fromJson(message);

        username = login.username;
        sendOK();
    }

    common::MessageHeader receiveHeader() const
    {
        std::vector<char> headerBytes(MESSAGE_HEADER_SIZE);
        socket.receive(headerBytes.data(), MESSAGE_HEADER_SIZE);

        return common::MessageHeader::deserialize(headerBytes);
    }

    void recieveOK() const
    {
        auto header = receiveHeader();

        std::cout << "Received OK message" << std::endl;

        if (header.messageType != MessageType::OK)
        {
            throw std::runtime_error("Expected OK message");
        }
    }
};
} // namespace common