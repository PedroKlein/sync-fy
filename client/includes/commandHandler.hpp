#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <string>

class CommandHandler
{
  public:
    using Command = std::function<void()>;

    void executeCommand(const std::string &command) const;

  protected:
    void registerCommand(const std::string &command, Command func);

  private:
    std::map<std::string, Command> commands;
};