#pragma once

#include "commandMessager.hpp"

class CommandHandler
{
  public:
    CommandHandler(const CommandMessager &messager);

    void executeCommand(std::string command, const std::vector<std::string> &parameters = {}) const;

  private:
    const CommandMessager &messager;

    void start(const std::string &parameter) const;
};
