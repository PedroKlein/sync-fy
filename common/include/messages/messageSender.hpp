#include "messageTypes.hpp"
#include "models/baseModel.h"
#include "socket/tcpSocket.hpp"
#include <iostream>

class MessageSender
{
  public:
    MessageSender(const TCPSocket &socket) : socket(socket)
    {
    }

    void sendMessage(MessageType type, const BaseModel &model)
    {
        auto message = buildJsonMessage(model.toJson(), type);
        socket.send(message);
    }

    void sendRawMessage(const std::vector<std::byte> &data, uint32_t step, uint32_t total)
    {
        auto message = buildRawDataMessage(data, step, total);
        socket.send(message);
    }

  protected:
    const TCPSocket &socket;

  private:
    std::vector<char> buildJsonMessage(const std::string &message, const MessageType id)
    {
        MessageHeader header(HeaderType::JSON_HEADER, id, message.size());
        std::vector<char> bytes = header.serialize();

        // Add message
        bytes.insert(bytes.end(), message.begin(), message.end());

        return bytes;
    }

    std::vector<char> buildRawDataMessage(const std::vector<std::byte> &data, uint32_t step, uint32_t total)
    {
        MessageHeader header(HeaderType::JSON_HEADER, id, message.size());
        std::vector<char> bytes = header.serialize();

        // Add raw data
        for (std::byte b : data)
        {
            bytes.push_back(static_cast<char>(b));
        }

        return bytes;
    }

    MessageHeader buildMessageHeader()
};