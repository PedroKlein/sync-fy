#include "localMonitor/messageHandler.hpp"

namespace localMonitor
{
void MessageHandler::handleMessage(const common::Message &message)
{
    std::cout << message << std::endl;
}

void MessageHandler::sendInitUploadFileMessage(const std::string &filename, size_t fileSize) const
{
    common::InitSendFile initSendFile(filename, fileSize);
    sendModelMessage(initSendFile);
}

void MessageHandler::sendFileMessage(common::File &file) const
{
    size_t totalSent = 0;
    const size_t fileSize = file.getSize();

    file.readFile([&](const common::FileChunk &chunk) {
        sendRawMessage(chunk.data, chunk.numPacket, chunk.totalPackets);
        totalSent += chunk.data.size();

        float progress = static_cast<float>(totalSent) / fileSize * 100;
        std::cout << "Progress: " << progress << "%\n";
    });
}

void MessageHandler::sendDeleteFileMessage(const std::string &filename) const
{
    common::DeleteFile deleteFile(filename);
    sendModelMessage(deleteFile);
}

} // namespace localMonitor