#pragma once

#include <filesystem/file.hpp>
#include <messages/message.hpp>
#include <messages/messageHandler.hpp>
#include <models/deleteFile.hpp>
#include <models/initReceiveFile.hpp>
#include <models/initSendFile.hpp>
#include <models/listFiles.hpp>

namespace common
{
class FileMessageHandler : public MessageHandler
{
  public:
    FileMessageHandler(TCPSocket &socket, std::string username, std::string syncFolder) : MessageHandler(socket)
    {
        this->username = username;
        this->syncFolder = syncFolder;
    }

    void sendFileMessage(File &file) const
    {
        sendInitSendFileMessage(file.getName(), file.getSize());
        sendFileDataMessage(file);
    }

    void receiveFileMessage() const
    {
        MessageHeader header = receiveHeader();

        if (header.messageType != common::MessageType::INIT_SEND_FILE)
        {
            throw std::runtime_error("Expected init send file message");
        }

        std::vector<char> messageData(header.dataSize);
        socket.receive(messageData.data(), header.dataSize);

        handleInitSendFile(messageData, header, true);
    }

    void sendDeleteFileMessage(const std::string &filename) const
    {
        DeleteFile deleteFile(filename);
        sendModelMessage(deleteFile);
    }

    const std::string &getUsername() const
    {
        return username;
    }

    const std::string &getSyncFolder() const
    {
        return syncFolder;
    }

  protected:
    std::string username;
    std::string syncFolder;

    virtual void handleOtherMessage(const Message &message) const {};

    virtual void onDeleteFileMessage(const DeleteFile &deleteFile) const {};
    virtual void onReceiveFileMessage(const InitReceiveFile &initReceiveFile) const {};
    virtual void onSendFileMessage(const InitSendFile &initSendFile) const {};
    virtual void onStartSendFileMessage(const InitSendFile &initSendFile) const {};

    virtual void onSendProgress(float progress) const {};

  private:
    void handleMessage(const Message &message) override
    {
        switch (message.getMessageHeader().messageType)
        {
        case MessageType::INIT_SEND_FILE:
            handleInitSendFile(message.getData(), message.getMessageHeader());
            break;
        case MessageType::INIT_RECEIVE_FILE:
            handleInitReceiveFile(message.getData(), message.getMessageHeader());
            break;
        case MessageType::DELETE_FILE:
            handleDeleteFile(message.getData());
            break;
        default:
            handleOtherMessage(message);
        }
    }

    void sendInitSendFileMessage(const std::string &filename, size_t fileSize) const
    {
        InitSendFile initSendFile(filename, fileSize);
        sendModelMessage(initSendFile);
    }

    void sendFileDataMessage(File &file) const
    {
        size_t totalSent = 0;
        const size_t fileSize = file.getSize();

        file.readFile([&](const FileChunk &chunk) {
            sendRawMessage(chunk.data, chunk.numPacket, chunk.totalPackets);
            totalSent += chunk.data.size();

            float progress = static_cast<float>(totalSent) / fileSize * 100;
            onSendProgress(progress);
        });
    }

    void handleInitReceiveFile(const std::vector<char> &data, MessageHeader header)
    {
        std::string message(data.begin(), data.end());
        InitReceiveFile initReceiveFile;
        initReceiveFile.fromJson(message);

        File file(syncFolder + initReceiveFile.filename);

        sendFileMessage(file);
    }

    void handleInitSendFile(const std::vector<char> &data, MessageHeader header, bool isCommand = false) const
    {
        std::string message(data.begin(), data.end());

        InitSendFile initSendFile;
        initSendFile.fromJson(message);

        onStartSendFileMessage(initSendFile);

        // for the specific scenario of download command (this is a gambiarra to prevent code duplication)
        std::string filePath = isCommand ? initSendFile.filename : syncFolder + initSendFile.filename;

        File file = File::create(filePath);

        if (initSendFile.fileSize > 0)
        {
            file.writeFile([&]() -> FileChunk {
                auto message = receiveRaw();
                auto messageHeader = message.getMessageHeader();
                return FileChunk(message.getData(), messageHeader.packet, messageHeader.totalPackets);
            });
        }

        onSendFileMessage(initSendFile);
    }

    void handleDeleteFile(const std::vector<char> &data)
    {
        std::string message(data.begin(), data.end());

        DeleteFile DeleteFile;
        DeleteFile.fromJson(message);

        std::string filePath = syncFolder + DeleteFile.filename;

        if (std::filesystem::exists(filePath))
        {
            std::filesystem::remove(filePath);
        }

        onDeleteFileMessage(DeleteFile);
    }
};
} // namespace common