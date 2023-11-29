#include "cli/messageHandler.hpp"

namespace cli
{
void MessageHandler::sendListServerFilesMessage() const
{
    // TODO: create message to request server files list
    // this does nothing
    common::ListFiles listFiles;
    sendModelMessage(listFiles);
}

void MessageHandler::onSendProgress(float progress) const
{
    std::cout << "Progress: " << progress << "%" << std::endl;
}

} // namespace cli