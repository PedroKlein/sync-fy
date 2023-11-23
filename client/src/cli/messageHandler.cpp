#include "cli/messageHandler.hpp"

namespace cli
{
void MessageHandler::handleJsonMessage(common::MessageHeader header, const std::string &message)
{
    std::cout << message << std::endl;
}

void MessageHandler::handleRawMessage(common::MessageHeader header)
{
    // auto bytes = socket.receive(header.dataSize);
    // std::string message(bytes.begin(), bytes.end());
    // std::cout << message << std::endl;
}

void MessageHandler::sendUploadFileMessage(const std::string &filename) const
{
    common::InitSendFile initSendFile(filename);
    sendModelMessage(initSendFile);
}

void MessageHandler::sendDownloadFileMessage(const std::string &filename) const
{
    common::InitRecieveFile initRecieveFile(filename);
    sendModelMessage(initRecieveFile);
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