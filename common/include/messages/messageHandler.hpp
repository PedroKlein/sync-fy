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
/**
* @class MessageHandler
* @brief The MessageHandler class handles communication with a TCP socket using a custom messaging protocol.
*/
class MessageHandler
{
  public:
    /**
    * @brief Constructs a MessageHandler object with the specified TCPSocket.
    * @param socket The TCPSocket to be used for communication.
    */
    MessageHandler(TCPSocket &socket) : socket(socket)
    {
    }

    /**
    * @brief Sends a message based on the provided BaseModel.
    * @param model The BaseModel used to construct the message.
    */
    void sendModelMessage(const BaseModel &model) const
    {
        Message message(model.getType(), model.toJson());
        sendMessage(message);
    }

    /**
    * @brief Sends a raw message (raw bytes) with the given data, packet number, and total number of packets.
    * @param data The raw data (bytes) to be sent.
    * @param numPacket The current packet number.
    * @param totalPackets The total number of packets for the message.
    */
    void sendRawMessage(const std::vector<char> &data, size_t numPacket = 1, size_t totalPackets = 1) const
    {
        Message message(common::MessageType::SEND_RAW, data, numPacket, totalPackets);
        sendMessage(message);
    }

    /**
     * @brief Sends a pure header message signalizing that an error happend.
     */
    void sendErrorMessage() const
    {
        Message message(common::MessageType::ERROR);
        sendMessage(message);
    }

    /**
    * @brief Receives a message from the connected socket.
    */
    void receiveMessage()
    {
        MessageHeader header = receiveHeader();

        if (header.headerType == common::HeaderType::PURE_HEADER)
        {
            handlePureHeaderMessage(header);
        }

        std::vector<char> messageData(header.dataSize);
        socket.receive(messageData.data(), header.dataSize);

        Message message(header, messageData);
        handleMessage(message);
    }

    /**
    * @brief Monitors incoming messages continuously until explicitly stopped.
    */
    void monitorMessages()
    {
        isMonitoring = true;

        do
        {
            receiveMessage();
        } while (isMonitoring);
    }

    /**
    * @brief Stops monitoring incoming messages.
    */
    void stopMonitoring()
    {
        isMonitoring = false;
    }

  protected:
    TCPSocket &socket;
    bool isMonitoring = false;
    
    /**
    * @brief Handles the received message.
    * @param message The received message.
    */
    virtual void handleMessage(const Message &message) = 0;

    /**
    * @brief Handles a pure header message without message data.
    * @param header The pure header message header.
    */
    virtual void handlePureHeaderMessage(const MessageHeader &header) const {};

    /**
    * @brief Receives a raw message from the connected socket.
    * @return The received raw message.
    */
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

    /**
    * @brief Receives a message header from the connected socket.
    * @return The received message header.
    */
    common::MessageHeader receiveHeader() const
    {
        std::vector<char> headerBytes(MESSAGE_HEADER_SIZE);
        socket.receive(headerBytes.data(), MESSAGE_HEADER_SIZE);

        return common::MessageHeader::deserialize(headerBytes);
    }

    /**
    * @brief Sends the provided message through the connected socket.
    * @param message The message to be sent.
    */
    void sendMessage(const Message &message) const
    {
        std::vector<char> serialized = message.serialize();

        socket.send(serialized.data(), serialized.size());
    }
};
} // namespace common