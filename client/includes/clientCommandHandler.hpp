#pragma once

#include "clientSocket.hpp"
#include "commandHandler.hpp"

class ClientCommandHandler : public CommandHandler
{
  public:
    ClientCommandHandler(const ClientSocket &socket);

    void start();
    void stop();
    void restart();

  private:
    const ClientSocket &socket;
};
