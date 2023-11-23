#pragma once

#include "messageHandler.hpp"
#include <filesystem/file.hpp>

namespace cli
{
// TODO: improve this class to use hashmaps instead of if-else statements
class CommandHandler
{
  public:
    CommandHandler(const MessageHandler &messageHandler);

    void executeCommand(std::string command, const std::vector<std::string> &parameters = {}) const;

  private:
    const MessageHandler &messageHandler;

    void upload(const std::string &filepath) const;
    void test() const;
};
} // namespace cli
