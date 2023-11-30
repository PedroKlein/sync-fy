#pragma once

#include "commandHandler.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <thread>

namespace cli
{
class CLI
{
  public:
    CLI(const CommandHandler &handler);
    ~CLI();

    std::thread *start();
    void stop();

  private:
    std::thread cliThread;
    bool isRunning;
    const CommandHandler &commandHandler;

    void run();
};
} // namespace cli
