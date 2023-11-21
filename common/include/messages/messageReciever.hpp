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
        char header[2];
        recv(sockfd, header, 2, 0);

        switch (header[0])
        {
        case 'H':
            uint32_t size;
            socket.receiveData(reinterpret_cast<char *>(&size), sizeof(size));
            recv(sockfd, reinterpret_cast<char *>(&size), sizeof(size), 0);

            uint8_t type;
            recv(sockfd, reinterpret_cast<char *>(&type), sizeof(type), 0);

            char *message = new char[size + 1];
            recv(sockfd, message, size, 0);
            message[size] = '\0';
            break;
        case 'F':
            uint32_t size;
            recv(sockfd, reinterpret_cast<char *>(&size), sizeof(size), 0);

            uint32_t step;
            recv(sockfd, reinterpret_cast<char *>(&step), sizeof(step), 0);

            uint32_t total;
            recv(sockfd, reinterpret_cast<char *>(&total), sizeof(total), 0);

            char *data = new char[size + 1];
            recv(sockfd, data, size, 0);
            data[size] = '\0';
            break;
        default:
            throw std::runtime_error("Invalid header");
            break;
        }

        // if (header[0] != 'H')
        // {
        //     // Handle invalid message
        //     return;
        // }

        // uint32_t size;
        // recv(sockfd, reinterpret_cast<char *>(&size), sizeof(size), 0);

        // uint8_t type;
        // recv(sockfd, reinterpret_cast<char *>(&type), sizeof(type), 0);

        // char *message = new char[size + 1];
        // recv(sockfd, message, size, 0);
        // message[size] = '\0';

        // handleMessage(static_cast<MessageType>(type), std::string(message));

        // delete[] message;
    }

  private:
    const TCPSocket &socket;

    // std::string readMessage()
    // {
    //     char header[10]; // Header size: 'H' + ' ' + 4 bytes (size) + ' ' + 1 byte (id) + '\n'
    //     recv(socket, header, sizeof(header), 0);

    //     uint32_t messageSize = 0;
    //     for (int i = 0; i < 4; ++i)
    //     {
    //         messageSize = (messageSize << 8) | (header[i + 2] & 0xFF); // Extract size from header
    //     }

    //     std::vector<char> messageData(messageSize);
    //     recv(socket, messageData.data(), messageSize, 0); // Receive message data

    //     return std::string(messageData.begin(), messageData.end());
    // }

    // std::vector<std::byte> readRawMessage()
    // {
    //     char
    //         header[14]; // Header size: 'F' + ' ' + 4 bytes (size) + ' ' + 4 bytes (step) + ' ' + 4 bytes (total) +
    //         '\n'
    //     recv(socket, header, sizeof(header), 0);

    //     uint32_t dataSize = 0;
    //     for (int i = 0; i < 4; ++i)
    //     {
    //         dataSize = (dataSize << 8) | (header[i + 2] & 0xFF); // Extract size from header
    //     }

    //     std::vector<std::byte> data(dataSize);
    //     recv(socket, data.data(), dataSize, 0); // Receive raw data

    //     return data;
    // }
};