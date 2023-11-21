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

    void sendMessage(const BaseModel &model) const
    {
        auto message = buildJsonMessage(model.toJson(), model.getType());
        socket.send(message);
    }

    void sendRawMessage(const std::vector<std::byte> &data)
    {
        auto message = buildRawDataMessage(data);
        socket.send(message);
    }

    void receiveMessage(bool shouldMonitor = true)
    {
        bool recievedMessage = false;
        while (shouldMonitor || !recievedMessage)
        {
            auto headerBytes = socket.receive(MESSAGE_HEADER_SIZE);
            if (headerBytes.size() > 0)
            {
                recievedMessage = true;
                MessageHeader header = MessageHeader::deserialize(headerBytes);
                switch (header.headerType)
                {
                case HeaderType::JSON_HEADER:
                    handleJsonMessage(header);
                    break;
                case HeaderType::RAW_DATA_HEADER:
                    handleRawMessage(header);
                    break;
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

    virtual void handleJsonMessage(MessageHeader header) = 0;

    virtual void handleRawMessage(MessageHeader header) = 0;

  private:
    std::vector<char> buildJsonMessage(const std::string &message, const MessageType id) const
    {
        MessageHeader header(HeaderType::JSON_HEADER, id, message.size());
        std::vector<char> bytes = header.serialize();

        // Add message
        bytes.insert(bytes.end(), message.begin(), message.end());

        return bytes;
    }

    std::vector<char> buildRawDataMessage(const std::vector<std::byte> &data)
    {
        MessageHeader header(HeaderType::JSON_HEADER, MessageType::SEND_RAW, data.size());
        std::vector<char> bytes = header.serialize();

        // Add raw data
        for (std::byte b : data)
        {
            bytes.push_back(static_cast<char>(b));
        }

        return bytes;
    }
};