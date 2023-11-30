#include "cli/messageHandler.hpp"

namespace cli
{
void MessageHandler::sendListServerFilesMessage() const
{
    common::Message initListFiles(common::MessageType::INIT_LIST_FILES);
    sendMessage(initListFiles);
}

std::vector<common::FileInfo> MessageHandler::receiveListFilesMessage() const
{
    common::MessageHeader header = receiveHeader();

    if (header.messageType != common::MessageType::LIST_FILES)
    {
        throw std::runtime_error("Expected list files message");
    }

    std::vector<char> messageData(header.dataSize);
    socket.receive(messageData.data(), header.dataSize);
    std::string message(messageData.begin(), messageData.end());

    common::ListFiles listFiles;
    listFiles.fromJson(message);

    sendOK();
    receiveOK();

    return listFiles.files;
}

void MessageHandler::onSendProgress(float progress) const
{
    std::cout << "Progress: " << progress << "%" << std::endl;
}

} // namespace cli