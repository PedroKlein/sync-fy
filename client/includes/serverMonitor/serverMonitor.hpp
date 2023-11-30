#pragma once

#include "serverMonitor/messageHandler.hpp"
#include <iostream>
#include <string>
#include <thread>

namespace serverMonitor
{
class ServerMonitor
{
  public:
    ServerMonitor(MessageHandler &messageHandler);
    ~ServerMonitor();

    std::thread *start();
    void stop();

  private:
    std::thread monitorThread;
    MessageHandler &messageHandler;

    void run();
};
} // namespace serverMonitor
