#pragma once

#include "messageTypes.hpp"
#include "models/baseModel.hpp"
#include "models/login.hpp"
#include "socket/tcpSocket.hpp"
#include <iostream>

namespace common
{
// TODO: Extract a Message class form this with all the logic related to the structure of our messages
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

    void sendData(const std::vector<char> &data, bool shouldConfirm = false) const
    {
        socket.send(data.data(), data.size());
        recieveOK();
    }

    void sendModelMessage(const BaseModel &model) const
    {
        auto message = buildJsonMessage(model.toJson(), model.getType());
        sendData(message);
    }

    void sendOK() const
    {
        MessageHeader header(HeaderType::PURE_HEADER, MessageType::OK, 0);
        std::vector<char> bytes = header.serialize();
        socket.send(bytes.data(), bytes.size());
    }

    void sendExit() const
    {
        MessageHeader header(HeaderType::PURE_HEADER, MessageType::EXIT, 0);
        std::vector<char> bytes = header.serialize();
        sendData(bytes);
    }

    void sendRawMessage(const std::vector<char> &data) const
    {
        auto message = buildRawDataMessage(data);
        sendData(message);
    }

    void receiveMessage()
    {
        MessageHeader header = receiveHeader();

        switch (header.headerType)
        {
        case HeaderType::JSON_HEADER:
            recieveJsonMessage(header);
            break;
        case HeaderType::RAW_DATA_HEADER:
            handleRawMessage(header);
            break;
        case HeaderType::PURE_HEADER:
            handlePureHeaderMessage(header);
        default:
            throw std::runtime_error("Invalid header");
            break;
        }

        sendOK();
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

    virtual void handleJsonMessage(MessageHeader header, const std::string &message) = 0;

    virtual void handleRawMessage(MessageHeader header) = 0;

    virtual void handleExitMessage() = 0;

  private:
    void handlePureHeaderMessage(MessageHeader header)
    {
        switch (header.messageType)
        {
        case MessageType::OK:
            std::cout << "Received OK message" << std::endl;
            break;
        case MessageType::EXIT:
            std::cout << "Received EXIT message" << std::endl;
            handleExitMessage();
            break;
        default:
            throw std::runtime_error("Invalid header");
            break;
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

        std::vector<char> messageBytes(header.dataSize);
        socket.receive(messageBytes.data(), header.dataSize);
        std::string message(messageBytes.begin(), messageBytes.end());

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

    void recieveJsonMessage(MessageHeader header)
    {
        std::vector<char> messageBytes(header.dataSize);
        socket.receive(messageBytes.data(), header.dataSize);
        std::string message(messageBytes.begin(), messageBytes.end());
        handleJsonMessage(header, message);
    }

    // void recieveRawMessage(MessageHeader header)
    // {
    //     auto messageBytes = socket.receive(header.dataSize);
    //     return messageBytes;
    // }

    std::vector<char> buildJsonMessage(const std::string &message, const MessageType id) const
    {
        MessageHeader header(HeaderType::JSON_HEADER, id, message.size());
        std::vector<char> bytes = header.serialize();

        bytes.insert(bytes.end(), message.begin(), message.end());
        return bytes;
    }

    std::vector<char> buildRawDataMessage(const std::vector<char> &data) const
    {
        MessageHeader header(HeaderType::RAW_DATA_HEADER, MessageType::SEND_RAW, data.size());
        std::vector<char> bytes = header.serialize();

        bytes.insert(bytes.end(), data.begin(), data.end());
        return bytes;
    }
};
} // namespace common