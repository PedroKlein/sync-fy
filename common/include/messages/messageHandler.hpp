#pragma once

#include "messageTypes.hpp"
#include "models/baseModel.hpp"
#include "socket/tcpSocket.hpp"
#include <iostream>

class MessageHandler
{
  public:
    MessageHandler(const TCPSocket &socket) : socket(socket)
    {
    }

    MessageHandler(const TCPSocket &socket, const std::string username) : socket(socket), username(username)
    {
    }

    void sendJsonMessage(const BaseModel &model) const
    {
        auto message = buildJsonMessage(model.toJson(), model.getType());
        socket.send(message.data(), message.size());
    }

    // void sendAcknowledge() const
    // {
    //     auto message = buildAcknowledgeMessage();
    //     socket.send(message.data(), message.size());
    // }

    void sendRawMessage(const std::vector<std::byte> &data)
    {
        auto message = buildRawDataMessage(data);
        socket.send(message.data(), message.size());
    }

    void receiveMessage(bool shouldMonitor = true)
    {
        bool recievedMessage = false;
        while (shouldMonitor || !recievedMessage)
        {
            std::vector<char> headerBytes(MESSAGE_HEADER_SIZE);
            socket.receive(headerBytes.data(), MESSAGE_HEADER_SIZE);

            if (headerBytes.size() > 0)
            {
                recievedMessage = true;
                MessageHeader header = MessageHeader::deserialize(headerBytes);
                switch (header.headerType)
                {
                case HeaderType::JSON_HEADER:
                    recieveJsonMessage(header);
                    break;
                case HeaderType::RAW_DATA_HEADER:
                    handleRawMessage(header);
                    break;
                // case HeaderType::PURE_HEADER:
                //     handlePureHeaderMessage(header);
                //     break;
                default:
                    throw std::runtime_error("Invalid header");
                    break;
                }
            }
        }
    }

    void setUsername(const std::string username)
    {
        this->username = username;
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

    // virtual void handlePureHeaderMessage(MessageHeader header) = 0;

  private:
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

        // Add message
        bytes.insert(bytes.end(), message.begin(), message.end());

        return bytes;
    }

    // std::vector<char> buildAcknowledgeMessage() const
    // {
    //     MessageHeader header(HeaderType::PURE_HEADER, MessageType::ACKNOWLEDGE, 0);
    //     std::vector<char> bytes = header.serialize();

    //     return bytes;
    // }

    std::vector<char> buildRawDataMessage(const std::vector<std::char> &data)
    {
        MessageHeader header(HeaderType::RAW_DATA_HEADER, MessageType::SEND_RAW, data.size());
        std::vector<char> bytes = header.serialize();

        // Add message
        bytes.insert(bytes.end(), data.begin(), data.end());

        return bytes;
    }
};