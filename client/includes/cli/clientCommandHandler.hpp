#pragma once

#include <iostream>

class ClientCommandHandler : public CommandHandler
{
  public:
    ClientCommandHandler()
    {
        registerCommand("start", std::bind(&ClientCommandHandler::start, this));
        registerCommand("stop", std::bind(&ClientCommandHandler::stop, this));
        registerCommand("restart", std::bind(&ClientCommandHandler::restart, this));
    }

    void start()
    {
        std::cout << "Start command executed." << std::endl;
        // Your start logic here
    }

    void stop()
    {
        std::cout << "Stop command executed." << std::endl;
        // Your stop logic here
    }

    void restart()
    {
        std::cout << "Restart command executed." << std::endl;
        // Your restart logic here
    }
};