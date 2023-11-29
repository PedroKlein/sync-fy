#pragma once

#include "clientMessageHandler.hpp"
#include <iostream>
#include <string>
#include <thread>

namespace serverMonitor
{
class ServerMonitor
{
  public:
    ServerMonitor(ClientMessageHandler &messageHandler);
    ~ServerMonitor();

    std::thread *start();
    void stop();

  private:
    std::thread monitorThread;
    ClientMessageHandler &messageHandler;

    void run();
};
} // namespace serverMonitor
