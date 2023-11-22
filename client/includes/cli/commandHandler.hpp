#pragma once

#include "messageHandler.hpp"

namespace cli
{
class CommandHandler
{
  public:
    CommandHandler(const MessageHandler &messager);

    void executeCommand(std::string command, const std::vector<std::string> &parameters = {}) const;

  private:
    const MessageHandler &messager;

    void start(const std::string &parameter) const;
};
} // namespace cli
