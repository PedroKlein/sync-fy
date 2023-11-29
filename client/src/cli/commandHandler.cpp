#include "cli/commandHandler.hpp"
#include <models/login.hpp>

namespace cli
{
CommandHandler::CommandHandler(const MessageHandler &messageHandler) : messageHandler(messageHandler)
{
}

void CommandHandler::executeCommand(std::string command, const std::vector<std::string> &parameters) const
{
    std::transform(command.begin(), command.end(), command.begin(), ::tolower); // Convert command to lowercase

    if (command == "upload")
    {
        upload(parameters[0]);
    }
    else if (command == "test")
    {
        test();
    }
    else
    {
        std::cout << "Unknown command." << std::endl;
    }
}

void CommandHandler::upload(const std::string &filepath) const
{
    std::cout << "Upload command start." << std::endl;
    common::File file(filepath);
    messageHandler.sendFileMessage(file);
}

void CommandHandler::test() const
{
    messageHandler.sendListServerFilesMessage();
}
} // namespace cli
