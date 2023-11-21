#pragma once

#include "cliMessageHandler.hpp"
#include "commandHandler.hpp"

class CLICommandHandler : public CommandHandler
{
  public:
    CLICommandHandler(const CLIMessageHandler &cliMessage);

    void upload();
    void stop();
    void restart();

  private:
    const CLIMessageHandler &cliMessage;
};
