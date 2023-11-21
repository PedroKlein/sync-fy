#include "messageTypes.hpp"
#include <iostream>

class MessageSender
{
  public:
    MessageSender(int socket) : socket(socket)
    {
    }

    void sendMessage(MessageType type, const std::string &data)
    {
        std::string message = buildMessage(data, type);
        send(socket, message.c_str(), message.size(), 0);
    }

    void sendRawMessage(const std::vector<std::byte> &data, uint32_t step, uint32_t total)
    {
        std::string message = buildRawDataMessage(data, step, total);
        send(socket, message.c_str(), message.size(), 0);
    }

  protected:
    int socket;

  private:
    // Header fotmat: H <size> <id>\n<data>
    std::vector<char> buildMessage(const std::string &message, const MessageType id)
    {
        std::vector<char> bytes;

        // Add header
        bytes.push_back('H');
        bytes.push_back(' ');

        // Add size as uint32_t
        uint32_t size = message.size();
        for (int i = sizeof(size) - 1; i >= 0; --i)
        {
            bytes.push_back((size >> (i * 8)) & 0xFF);
        }
        bytes.push_back(' ');

        // Add id
        bytes.push_back(static_cast<uint8_t>(id));
        bytes.push_back('\n');

        // Add message
        bytes.insert(bytes.end(), message.begin(), message.end());

        return bytes;
    }

    // Header fotmat: F <size> <step> <total>\n<data>
    std::vector<char> buildRawDataMessage(const std::vector<std::byte> &data, uint32_t step, uint32_t total)
    {
        std::vector<char> bytes;

        // Add header
        bytes.push_back('F');
        bytes.push_back(' ');

        // Add size of data as uint32_t
        uint32_t dataSize = data.size();
        for (int i = sizeof(dataSize) - 1; i >= 0; --i)
        {
            bytes.push_back((dataSize >> (i * 8)) & 0xFF);
        }
        bytes.push_back(' ');

        // Add step as uint32_t
        for (int i = sizeof(step) - 1; i >= 0; --i)
        {
            bytes.push_back((step >> (i * 8)) & 0xFF);
        }
        bytes.push_back(' ');

        // Add total as uint32_t
        for (int i = sizeof(total) - 1; i >= 0; --i)
        {
            bytes.push_back((total >> (i * 8)) & 0xFF);
        }
        bytes.push_back('\n');

        // Add raw data
        for (std::byte b : data)
        {
            bytes.push_back(static_cast<char>(b));
        }

        return bytes;
    }
};