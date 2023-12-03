#pragma once

#include "commandHandler.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <thread>

namespace cli
{
/**
 * @class CLI
 * @brief Represents a Command Line Interface (CLI) for interacting with the application.
 */
class CLI
{
  public:
    /**
     * @brief Constructor for CLI.
     * @param handler The CommandHandler to handle commands from the CLI.
     */
    CLI(const CommandHandler &handler);
    
    /**
     * @brief Destructor for CLI.
     */
    ~CLI();

    /**
     * @brief Starts the CLI in a separate thread.
     * @return A pointer to the thread handling the CLI.
     */
    std::thread *start();

    /**
     * @brief Stops the CLI.
     */
    void stop();

  private:
    std::thread cliThread;
    bool isRunning;
    const CommandHandler &commandHandler;

    /**
     * @brief Internal function to run the CLI.
     */
    void run();
};
} // namespace cli
