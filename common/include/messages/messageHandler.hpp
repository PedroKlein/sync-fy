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
        receiveLoginMessage();

        syncFolder = DEFAULT_SERVER_SYNC_DIR + username + "/";
        std::cout << "Logged in as " << username << std::endl;
    }

    MessageHandler(const TCPSocket &socket, const std::string username)
        : socket(socket), username(username), syncFolder(DEFAULT_CLIENT_SYNC_DIR)
    {
        sendLoginMessage(username);
    }

    void sendModelMessage(const BaseModel &model) const
    {
        Message message(model.getType(), model.toJson());
        sendMessage(message);
    }

    void sendOK() const
    {
        Message okMessage(common::MessageType::OK);
        sendMessage(okMessage, false);
    }

    void sendExit() const
    {
        Message exitMessage(common::MessageType::EXIT);
        sendMessage(exitMessage);
    }

    void sendRawMessage(const std::vector<char> &data, size_t numPacket = 1, size_t totalPackets = 1) const
    {
        Message message(common::MessageType::SEND_RAW, data, numPacket, totalPackets);
        sendMessage(message, false);
    }

    void receiveMessage()
    {
        MessageHeader header = receiveHeader();

        if (header.headerType == common::HeaderType::PURE_HEADER)
        {
            Message message(header);
            sendOK();
            handleMessage(message);
            return;
        }

        std::vector<char> messageData(header.dataSize);
        socket.receive(messageData.data(), header.dataSize);

        // TODO: Refactor this ifs
        if (header.messageType == MessageType::INIT_SEND_FILE)
        {
            handleInitSendFile(messageData, header);
            return;
        }

        if (header.messageType == MessageType::INIT_RECEIVE_FILE)
        {
            handleInitReceiveFile(messageData, header);
            return;
        }

        if (header.messageType == MessageType::DELETE_FILE)
        {
            handleDeleteFile(messageData);
            return;
        }

        Message message(header, messageData);
        sendOK();
        handleMessage(message);
    }

    void monitorMessages()
    {
        isMonitoring = true;
        while (isMonitoring)
        {
            receiveMessage();
        }
    }

    void receiveOK() const
    {
        auto header = receiveHeader();

        std::cout << "Received OK message" << std::endl;

        if (header.messageType != MessageType::OK)
        {
            throw std::runtime_error("Expected OK message");
        }
    }

    const std::string &getUsername() const
    {
        return username;
    }

    void stopMonitoring()
    {
        isMonitoring = false;
    }

  protected:
    const TCPSocket &socket;
    std::string username;
    bool isMonitoring = false;
    std::string syncFolder;

    virtual void handleMessage(const Message &message) = 0;

  private:
    void initSyncFolder()
    {
        if (!std::filesystem::exists(syncFolder))
        {
            try
            {
                std::filesystem::create_directory(syncFolder);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error creating directory: " << e.what() << std::endl;
            }
        }
    }

    void sendMessage(const Message &message, bool waitForResponse = true) const
    {
        std::vector<char> serialized = message.serialize();

        socket.send(serialized.data(), serialized.size());

        if (waitForResponse)
        {
            receiveOK();
        }
    }

    void sendLoginMessage(const std::string username)
    {
        common::Login login(username);
        sendModelMessage(login);
    }

    void receiveLoginMessage()
    {
        MessageHeader header = receiveHeader();

        if (header.messageType != MessageType::LOGIN)
        {
            throw std::runtime_error("Expected login message");
        }

        std::vector<char> messageData(header.dataSize);
        socket.receive(messageData.data(), header.dataSize);
        std::string message(messageData.begin(), messageData.end());

        Login login;
        login.fromJson(message);

        username = login.username;
        sendOK();
    }

    Message receiveRaw()
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

    void handleInitReceiveFile(const std::vector<char> &data, common::MessageHeader header)
    {
        // filename = std::string(data.begin(), data.end());
        // std::cout << "Receiving file " << filename << std::endl;
        sendOK();
    }

    void handleInitSendFile(const std::vector<char> &data, common::MessageHeader header)
    {
        std::string message(data.begin(), data.end());

        InitSendFile initSendFile;
        initSendFile.fromJson(message);

        sendOK();

        initSyncFolder();

        File file = File::create(syncFolder + "/" + initSendFile.filename);

        if (initSendFile.fileSize == 0)
        {
            sendOK();
            return;
        }

        file.writeFile([&]() -> common::FileChunk {
            auto message = receiveRaw();
            auto messageHeader = message.getMessageHeader();
            return common::FileChunk(message.getData(), messageHeader.packet, messageHeader.totalPackets);
        });

        sendOK();
    }

    void handleDeleteFile(const std::vector<char> &data)
    {
        std::string message(data.begin(), data.end());

        DeleteFile DeleteFile;
        DeleteFile.fromJson(message);

        std::string filePath = syncFolder + "/" + DeleteFile.filename;

        if (std::filesystem::exists(filePath))
        {
            std::filesystem::remove(filePath);
        }

        sendOK();

        // TODO: propagate delete to other clients
    }
};
} // namespace common