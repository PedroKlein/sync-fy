#pragma once

#include "constants.hpp"
#include "filesystem/file.hpp"
#include "message.hpp"
#include "messageTypes.hpp"
#include "models/baseModel.hpp"
#include "models/deleteFile.hpp"
#include "models/initReceiveFile.hpp"
#include "models/initSendFile.hpp"
#include "models/login.hpp"
#include "socket/tcpSocket.hpp"
#include <filesystem>
#include <iostream>
#include <sys/stat.h>

namespace common
{
class MessageHandler
{
  public:
    MessageHandler(const TCPSocket &socket) : socket(socket)
    {
    }

    void sendModelMessage(const BaseModel &model) const
    {
        Message message(model.getType(), model.toJson());
        sendMessage(message);
    }

    void sendOK() const
    {
        Message okMessage(common::MessageType::OK);
        sendMessage(okMessage);
    }

    void sendExit() const
    {
        Message exitMessage(common::MessageType::EXIT);
        sendMessage(exitMessage);
    }

    void sendRawMessage(const std::vector<char> &data, size_t numPacket = 1, size_t totalPackets = 1) const
    {
        Message message(common::MessageType::SEND_RAW, data, numPacket, totalPackets);
        sendMessage(message);
    }

    void receiveMessage()
    {
        MessageHeader header = receiveHeader();

        if (header.headerType == common::HeaderType::PURE_HEADER)
        {
            if (header.messageType == common::MessageType::EXIT)
            {
                onExit();
                isMonitoring = false;
                return;
            }
            handlePureHeaderMessage(header);
        }

        std::vector<char> messageData(header.dataSize);
        socket.receive(messageData.data(), header.dataSize);

        Message message(header, messageData);
        handleMessage(message);
    }

    void monitorMessages()
    {
        isMonitoring = true;

        do
        {
            receiveMessage();
        } while (isMonitoring);
    }

    void receiveOK() const
    {
        auto header = receiveHeader();

        if (header.messageType != MessageType::OK)
        {
            throw std::runtime_error("Expected OK message");
        }
    }

    void stopMonitoring()
    {
        isMonitoring = false;
    }

  protected:
    const TCPSocket &socket;
    bool isMonitoring = false;

    virtual void handleMessage(const Message &message) = 0;
    virtual void handlePureHeaderMessage(const MessageHeader &header) const {};
    virtual void onExit(){};

    Message receiveRaw() const
    {
        MessageHeader header = receiveHeader();

        if (header.messageType != MessageType::SEND_RAW)
        {
            throw std::runtime_error("Expected raw message");
        }

        std::vector<char> messageData(header.dataSize);
        socket.receive(messageData.data(), header.dataSize);

        return Message(header, messageData);
    }

    common::MessageHeader receiveHeader() const
    {
        std::vector<char> headerBytes(MESSAGE_HEADER_SIZE);
        socket.receive(headerBytes.data(), MESSAGE_HEADER_SIZE);

        return common::MessageHeader::deserialize(headerBytes);
    }

    void sendMessage(const Message &message) const
    {
        std::vector<char> serialized = message.serialize();

        socket.send(serialized.data(), serialized.size());
    }
};
} // namespace common