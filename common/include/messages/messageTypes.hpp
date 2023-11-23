#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

namespace common
{
constexpr size_t MESSAGE_HEADER_SIZE =
    sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);

enum MessageType
{
    OK,
    EXIT,
    INIT_SEND_FILE,
    INIT_RECEIVE_FILE,
    SEND_RAW,
    LOGIN,
    DELETE_FILE,
    LIST_FILES,
    LIST_SERVER,
    LIST_CLIENT,
};

enum HeaderType
{
    JSON_HEADER = 'J',
    RAW_DATA_HEADER = 'R',
    PURE_HEADER = 'H',
};

struct MessageHeader
{
    HeaderType headerType;
    MessageType messageType;
    uint32_t dataSize;
    uint32_t packet;
    uint32_t totalPackets;

    MessageHeader(HeaderType headerType, MessageType messageType, uint32_t dataSize, uint32_t packet,
                  uint32_t totalPackets)
        : headerType(headerType), messageType(messageType), dataSize(dataSize), packet(packet),
          totalPackets(totalPackets)
    {
    }

    std::vector<char> serialize() const
    {
        std::vector<char> bytes;

        // Add header type
        bytes.push_back(headerType);

        // Add message type
        bytes.push_back(static_cast<uint8_t>(messageType));

        // Add data size as uint32_t
        for (int i = sizeof(dataSize) - 1; i >= 0; --i)
        {
            bytes.push_back((dataSize >> (i * 8)) & 0xFF);
        }

        // Add packet num as uint32_t
        for (int i = sizeof(packet) - 1; i >= 0; --i)
        {
            bytes.push_back((packet >> (i * 8)) & 0xFF);
        }

        // Add total packets as uint32_t
        for (int i = sizeof(totalPackets) - 1; i >= 0; --i)
        {
            bytes.push_back((totalPackets >> (i * 8)) & 0xFF);
        }

        return bytes;
    }

    static MessageHeader deserialize(std::vector<char> bytes)
    {
        size_t offset = 0;
        HeaderType headerType = static_cast<HeaderType>(bytes[offset]);
        offset++;
        MessageType messageType = static_cast<MessageType>(bytes[offset]);
        offset++;

        uint32_t dataSize = 0;
        for (int i = 0; i < sizeof(uint32_t); ++i)
        {
            dataSize = (dataSize << 8) | (bytes[i + offset] & 0xFF); // Extract size from header
        }
        offset += sizeof(uint32_t);

        uint32_t packet = 0;
        for (int i = 0; i < sizeof(uint32_t); ++i)
        {
            packet = (packet << 8) | (bytes[i + offset] & 0xFF); // Extract packet from header
        }
        offset += sizeof(uint32_t);

        uint32_t totalPackets = 0;
        for (int i = 0; i < sizeof(uint32_t); ++i)
        {
            totalPackets = (totalPackets << 8) | (bytes[i + offset] & 0xFF); // Extract totalPackets from header
        }

        return MessageHeader(headerType, messageType, dataSize, packet, totalPackets);
    }

    friend std::ostream &operator<<(std::ostream &os, const MessageHeader &header)
    {
        os << "Header Type: " << static_cast<char>(header.headerType) << "\n";
        os << "Message Type: " << static_cast<int>(header.messageType) << "\n";
        os << "Data Size: " << header.dataSize << "\n";
        os << "Packet: " << header.packet << "\n";
        os << "Total Packets: " << header.totalPackets << "\n";
        return os;
    }
};
} // namespace common