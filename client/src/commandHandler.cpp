#include "commandHandler.hpp"

void CommandHandler::executeCommand(const std::string &command) const
{
    if (commands.find(command) != commands.end())
    {
        commands.at(command)();
    }
    else
    {
        // Handle unknown command
        std::cout << "Unknown command: " << command << std::endl;
    }
}

void CommandHandler::registerCommand(const std::string &command, Command func)
{
    commands[command] = func;
}