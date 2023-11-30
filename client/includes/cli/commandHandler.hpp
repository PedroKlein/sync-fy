#pragma once

#include "messageHandler.hpp"
#include <filesystem/directory.hpp>
#include <filesystem/file.hpp>

namespace cli
{
class CommandHandler
{
  public:
    CommandHandler(const MessageHandler &messageHandler);

    void executeCommand(std::string command, const std::vector<std::string> &parameters = {}) const;

  private:
    const MessageHandler &messageHandler;
    std::map<std::string, std::function<void(const std::vector<std::string> &)>> commands;
    const common::Directory directory;

    void registerCommand(const std::string &command, std::function<void(const std::vector<std::string> &)> method);

    void upload(const std::string &filepath) const;
    void deleteFile(const std::string &filename) const;
    void listFiles(const std::string &from) const;
};
} // namespace cli
