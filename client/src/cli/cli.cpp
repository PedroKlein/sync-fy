#include "cli/cli.hpp"

namespace cli
{
CLI::CLI(const CommandHandler &handler) : isRunning(false), commandHandler(handler)
{
}

CLI::~CLI()
{
    stop();
    std::cout << "CLI stopped" << std::endl;
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
}

void CLI::run()
{
    struct pollfd pfd = {STDIN_FILENO, POLLIN, 0};
    std::string input;
    std::cout << "Enter command: " << std::flush;

    do
    {
        if (poll(&pfd, 1, 0) > 0) // Check if there's input
        {
            std::string input;
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
            std::cout << "Enter command: " << std::flush;
        }
    } while (isRunning);
}
} // namespace cli