#pragma once

#include <cstdint>
#include <vector>

#define MESSAGE_HEADER_SIZE 9

enum MessageType
{
    INIT_SEND_FILE,
    SEND_RAW,
    LOGIN,
    DELETE_FILE,
    LIST_SERVER,
    LIST_CLIENT,
};

enum HeaderType
{
    JSON_HEADER = 'J',
    RAW_DATA_HEADER = 'R',
};

struct MessageHeader
{
    HeaderType headerType;
    MessageType messageType;
    uint32_t dataSize;

    MessageHeader(HeaderType headerType, MessageType messageType, uint32_t dataSize)
        : headerType(headerType), messageType(messageType), dataSize(dataSize)
    {
    }

    std::vector<char> serialize()
    {
        std::vector<char> bytes;

        // Add header type
        bytes.push_back(headerType);
        bytes.push_back(' ');

        // Add message type
        bytes.push_back(static_cast<uint8_t>(messageType));
        bytes.push_back(' ');

        // Add data size as uint32_t
        for (int i = sizeof(dataSize) - 1; i >= 0; --i)
        {
            bytes.push_back((dataSize >> (i * 8)) & 0xFF);
        }
        bytes.push_back('\n');

        return bytes;
    }

    static MessageHeader deserialize(std::vector<char> bytes)
    {
        HeaderType headerType = static_cast<HeaderType>(bytes[0]);
        MessageType messageType = static_cast<MessageType>(bytes[2]);

        uint32_t dataSize = 0;
        for (int i = 0; i < 4; ++i)
        {
            dataSize = (dataSize << 8) | (bytes[i + 4] & 0xFF); // Extract size from header
        }

        return MessageHeader(headerType, messageType, dataSize);
    }
};