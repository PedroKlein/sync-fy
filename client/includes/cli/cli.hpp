#pragma once

#include "commandHandler.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <thread>

class CLI
{
  public:
    CLI(CommandHandler handler) : isRunning(false)
    {
        setCommandHandler(handler);
    }

    ~CLI()
    {
        stop();
    }

    void start()
    {
        isRunning = true;
        cliThread = std::thread(&CLI::run, this);
    }
    void stop()
    {
        isRunning = false;
        if (cliThread.joinable())
        {
            cliThread.join();
        }
    }

    void setCommandHandler(CommandHandler handler)
    {
        commandHandler = handler;
    }

  private:
    std::thread cliThread;
    bool isRunning;
    CommandHandler commandHandler;

    void run()
    {
        while (isRunning)
        {
            std::string command;
            std::cout << "Enter command: ";
            std::getline(std::cin, command);

            commandHandler.executeCommand(command);
        }
    }
};
