#include "cli/commandHandler.hpp"
#include <models/login.hpp>

CommandHandler::CommandHandler(const CommandMessager &messager) : messager(messager)
{
}

void CommandHandler::executeCommand(std::string command, const std::vector<std::string> &parameters) const
{
    std::transform(command.begin(), command.end(), command.begin(), ::tolower); // Convert command to lowercase

    if (command == "start")
    {
        start(parameters[0]);
    }
    else
    {
        std::cout << "Unknown command." << std::endl;
    }
}

void CommandHandler::start(const std::string &parameter) const
{
    std::cout << "Start command executed." << std::endl;
    std::vector<char> message(std::begin("start"), std::end("start") - 1);
    Login login(messager.getUsername());
    messager.sendMessage(login);
}
