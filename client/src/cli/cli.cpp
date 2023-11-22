#include "cli/cli.hpp"

namespace cli
{
CLI::CLI(const CommandHandler &handler) : isRunning(false), commandHandler(handler)
{
}

CLI::~CLI()
{
    stop();
}

std::thread *CLI::start()
{
    isRunning = true;
    cliThread = std::thread(&CLI::run, this);
    return &cliThread;
}

void CLI::stop()
{
    isRunning = false;
    if (cliThread.joinable())
    {
        cliThread.join();
    }
}

void CLI::run()
{
    while (isRunning)
    {
        std::string command;
        std::cout << "Enter command: ";
        std::getline(std::cin, command);

        commandHandler.executeCommand(command);
    }
}
} // namespace cli