#pragma once

#include "messageTypes.hpp"
#include "models/baseModel.hpp"
#include "models/login.hpp"
#include <iostream>

namespace common
{
/**
* @class Message
* @brief Represents a message with a header and optional data.
*/
class Message
{
  public:
    /**
    * @brief Constructs a Message with a specified MessageType.
    * @param messageType The type of the message.
    */
    Message(MessageType messageType) : header(HeaderType::PURE_HEADER, messageType, 0, 1, 1)
    {
    }

    /**
    * @brief Constructs a Message with a specified MessageType and data in string format.
    * @param messageType The type of the message.
    * @param data The data in string format.
    */
    Message(MessageType messageType, const std::string &data)
        : header(HeaderType::JSON_HEADER, messageType, data.size(), 1, 1), data(data.begin(), data.end())
    {
    }

    /**
     * @brief Constructs a Message with a specified MessageType and data in vector<char> format.
     * @param messageType The type of the message.
     * @param data The data in vector<char> format.
     */
    Message(MessageType messageType, const std::vector<char> &data)
        : header(HeaderType::JSON_HEADER, messageType, data.size(), 1, 1), data(data)
    {
    }

    /**
     * @brief Constructs a Message with a specified MessageType, data, packet, and totalPackets.
     * @param messageType The type of the message.
     * @param data The data in vector<char> format.
     * @param packet The packet number.
     * @param totalPackets The total number of packets.
     */
    Message(MessageType messageType, const std::vector<char> &data, size_t packet, size_t totalPackets)
        : header(HeaderType::RAW_DATA_HEADER, messageType, data.size(), packet, totalPackets), data(data)
    {
    }

    /**
     * @brief Constructs a Message with a specified MessageHeader and optional data.
     * @param messageHeader The header of the message.
     * @param data The data in vector<char> format.
     */
    Message(MessageHeader messageHeader, const std::vector<char> &data = {}) : header(messageHeader), data(data)
    {
    }

    /**
     * @brief Serializes the Message into a vector of characters.
     * @return A vector of characters representing the serialized message.
     */
    std::vector<char> serialize() const
    {
        std::vector<char> bytes = header.serialize();

        if (header.headerType != HeaderType::PURE_HEADER)
        {
            bytes.insert(bytes.end(), data.begin(), data.end());
        }

        return bytes;
    }

    /**
     * @brief Gets the header of the Message.
     * @return The MessageHeader object representing the header of the message.
     */
    MessageHeader getMessageHeader() const
    {
        return header;
    }

    /**
     * @brief Gets the data of the Message.
     * @return A vector of characters representing the data of the message.
     */
    const std::vector<char> getData() const
    {
        return data;
    }

    friend std::ostream &operator<<(std::ostream &os, const Message &msg)
    {
        os << "Message Header: " << msg.header << "\n";
        os << "Data: ";
        for (char c : msg.data)
        {
            os << c;
        }
        return os;
    }

  private:
    const MessageHeader header;
    const std::vector<char> data;
};
} // namespace common