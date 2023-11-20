#pragma once

#include <functional>
#include <map>
#include <string>

class CommandHandler
{
  public:
    using Command = std::function<void()>;

    void executeCommand(const std::string &command)
    {
        if (commands.find(command) != commands.end())
        {
            commands[command]();
        }
        else
        {
            // Handle unknown command
            std::cout << "Unknown command: " << command << std::endl;
        }
    }

  protected:
    void registerCommand(const std::string &command, Command func)
    {
        commands[command] = func;
    }

  private:
    std::map<std::string, Command> commands;
};