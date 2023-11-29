#include "cli/commandHandler.hpp"
#include <models/login.hpp>

namespace cli
{
CommandHandler::CommandHandler(const MessageHandler &messageHandler) : messageHandler(messageHandler)
{
    registerCommand("upload", [this](const std::vector<std::string> &parameters) { upload(parameters[0]); });
    registerCommand("delete", [this](const std::vector<std::string> &parameters) { deleteFile(parameters[0]); });
}

void CommandHandler::executeCommand(std::string command, const std::vector<std::string> &parameters) const
{
    std::transform(command.begin(), command.end(), command.begin(), ::tolower); // Convert command to lowercase

    if (commands.find(command) != commands.end())
    {
        commands.at(command)(parameters);
    }
    else
    {
        std::cout << "Command not found." << std::endl;
    }
}

void CommandHandler::upload(const std::string &filepath) const
{
    std::cout << "Upload command start." << std::endl;
    common::File file(filepath);
    messageHandler.sendFileMessage(file);
}

void CommandHandler::deleteFile(const std::string &filename) const
{
    messageHandler.sendDeleteFileMessage(filename);
}

void CommandHandler::registerCommand(const std::string &command,
                                     std::function<void(const std::vector<std::string> &)> method)
{
    commands.insert(std::make_pair(command, method));
}
} // namespace cli
