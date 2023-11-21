#pragma once

#include "commandHandler.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <thread>

class CLI
{
  public:
    CLI(const CommandHandler &handler);
    ~CLI();

    void start();
    void stop();

  private:
    std::thread cliThread;
    bool isRunning;
    const CommandHandler &commandHandler;

    void run();
};
