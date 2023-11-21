#include "messageTypes.hpp"
#include "socket/tcpSocket.hpp"
#include <string>
#include <unistd.h>

class MessageReceiver
{

  public:
    MessageReceiver(const TCPSocket &socket) : socket(socket)
    {
    }

    void receiveMessage()
    {
        auto headerBytes = socket.receive(MESSAGE_HEADER_SIZE);
        MessageHeader header = MessageHeader::deserialize(headerBytes);

        switch (header.headerType)
        {
        case HeaderType.JSON_HEADER:
            handleJsonMessage(header);
            break;
        case HeaderType.RAW_DATA_HEADER:
            handleRawMessage(header);
            break;
        default:
            throw std::runtime_error("Invalid header");
            break;
        }
    }

    virtual void handleJsonMessage(MessageHeader header) = 0;

    virtual void handleRawMessage(MessageHeader header) = 0;

  protected:
    const TCPSocket &socket;
};