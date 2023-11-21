#pragma once

#include "commandHandler.hpp"
#include "socket.hpp"

class ClientCommandHandler : public CommandHandler
{
  public:
    ClientCommandHandler(const Socket &socket);

    void start();
    void stop();
    void restart();

  private:
    const Socket &socket;
};
