#include "cli/cli.hpp"

CLI::CLI(const CommandHandler &handler) : isRunning(false), commandHandler(handler)
{
}

CLI::~CLI()
{
    stop();
}

void CLI::start()
{
    isRunning = true;
    cliThread = std::thread(&CLI::run, this);
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
