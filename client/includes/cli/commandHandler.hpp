#pragma once

#include "messageHandler.hpp"
#include <datetime/dateTime.hpp>
#include <filesystem/directory.hpp>
#include <filesystem/file.hpp>
#include <stdlib.h>

namespace cli
{
/**
 * @class CommandHandler
 * @brief Handles and executes commands for the command-line interface.
 */
class CommandHandler
{
  public:
    /**
     * @brief Constructor for CommandHandler.
     * @param messageHandler The MessageHandler to use for displaying messages.
     */
    CommandHandler(const MessageHandler &messageHandler);

    /**
     * @brief Executes a command with optional parameters.
     * @param command The command to execute.
     * @param parameters Optional parameters for the command.
     */
    void executeCommand(std::string command, const std::vector<std::string> &parameters = {}) const;

  private:
    const MessageHandler &messageHandler;
    std::map<std::string, std::function<void(const std::vector<std::string> &)>> commands;
    const common::Directory directory;

    /**
     * @brief Registers a command with its corresponding function.
     * @param command The command to register.
     * @param method The function to associate with the command.
     */
    void registerCommand(const std::string &command, std::function<void(const std::vector<std::string> &)> method);

    /**
     * @brief Uploads a file.
     * @param filepath The path of the file to upload.
     */
    void upload(const std::string &filepath) const;

    /**
     * @brief Downloads a file.
     * @param filename The name of the file to download.
     */
    void download(const std::string &filename) const;
    
    /**
     * @brief Deletes a file.
     * @param filename The name of the file to delete.
     */
    void deleteFile(const std::string &filename) const;

    /**
     * @brief Lists files from a specified directory.
     * @param from The directory from which to list files.
     */
    void listFiles(const std::string &from) const;
    
    /**
     * @brief Exits the command-line interface.
     */
    void exit() const;
};
} // namespace cli
