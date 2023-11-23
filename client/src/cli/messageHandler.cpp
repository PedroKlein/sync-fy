#include "cli/messageHandler.hpp"

namespace cli
{
void MessageHandler::handleMessage(const common::Message &message)
{
    std::cout << message << std::endl;
}

void MessageHandler::sendInitUploadFileMessage(const std::string &filename) const
{
    common::InitSendFile initSendFile(filename);
    sendModelMessage(initSendFile);
}

void MessageHandler::sendFileMessage(common::File &file)
{
    size_t totalSent = 0;
    const size_t fileSize = file.getSize();

    file.sendFile([&](const std::vector<char> &chunk) {
        sendRawMessage(chunk);
        totalSent += chunk.size();

        float progress = static_cast<float>(totalSent) / fileSize * 100;
        std::cout << "Progress: " << progress << "%\n";
    });
}

void MessageHandler::sendDownloadFileMessage(const std::string &filename) const
{
    common::InitReceiveFile initReceiveFile(filename);
    sendModelMessage(initReceiveFile);
}

void MessageHandler::sendDeleteFileMessage(const std::string &filename) const
{
    common::DeleteFile deleteFile(filename);
    sendModelMessage(deleteFile);
}
void MessageHandler::sendListServerFilesMessage() const
{
    common::ListFiles listFiles;
    sendModelMessage(listFiles);
}
void MessageHandler::sendListClientFilesMessage() const
{
    common::ListFiles listFiles;
    sendModelMessage(listFiles);
}

} // namespace cli