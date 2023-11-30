#pragma once

#include "messageTypes.hpp"
#include "models/baseModel.hpp"
#include "models/login.hpp"
#include <iostream>

namespace common
{
class Message
{
  public:
    Message(MessageType messageType) : header(HeaderType::PURE_HEADER, messageType, 0, 1, 1)
    {
    }

    Message(MessageType messageType, const std::string &data)
        : header(HeaderType::JSON_HEADER, messageType, data.size(), 1, 1), data(data.begin(), data.end())
    {
    }

    Message(MessageType messageType, const std::vector<char> &data)
        : header(HeaderType::JSON_HEADER, messageType, data.size(), 1, 1), data(data)
    {
    }

    Message(MessageType messageType, const std::vector<char> &data, size_t packet, size_t totalPackets)
        : header(HeaderType::RAW_DATA_HEADER, messageType, data.size(), packet, totalPackets), data(data)
    {
    }

    Message(MessageHeader messageHeader, const std::vector<char> &data = {}) : header(messageHeader), data(data)
    {
    }

    std::vector<char> serialize() const
    {
        std::vector<char> bytes = header.serialize();

        if (header.headerType != HeaderType::PURE_HEADER)
        {
            bytes.insert(bytes.end(), data.begin(), data.end());
        }

        return bytes;
    }

    MessageHeader getMessageHeader() const
    {
        return header;
    }

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