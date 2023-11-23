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
        std::string input;
        std::cout << "Enter command: ";
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string word;

        // Get the command
        std::getline(iss, word, ' ');
        std::string command = word;

        // Get the parameters
        std::vector<std::string> parameters;
        while (std::getline(iss, word, ' '))
        {
            parameters.push_back(word);
        }

        commandHandler.executeCommand(command, parameters);
    }
}
} // namespace cli