#pragma once

#include "../socket/socket.hpp"
#include <iostream>

class ClientCommandHandler : public CommandHandler
{
  public:
    ClientCommandHandler(const Socket &socket) : socket(socket)
    {
        registerCommand("start", std::bind(&ClientCommandHandler::start, this));
        registerCommand("stop", std::bind(&ClientCommandHandler::stop, this));
        registerCommand("restart", std::bind(&ClientCommandHandler::restart, this));
    }

    void start()
    {
        std::cout << "Start command executed." << std::endl;
        socket.SendData("start");
        // Your start logic here
    }

    void stop()
    {
        std::cout << "Stop command executed." << std::endl;
        socket.SendData("stop");
        // Your stop logic here
    }

    void restart()
    {
        std::cout << "Restart command executed." << std::endl;
        socket.SendData("restart");
        // Your restart logic here
    }

  private:
    const Socket &socket;
};