#pragma once

#include "backupMessageHandler.hpp"
#include <iostream>
#include <string>
#include <thread>

namespace backup
{
class PrimaryMonitor
{
  public:
    PrimaryMonitor(BackupMessageHandler &messageHandler) : messageHandler(messageHandler)
    {
    }

    ~PrimaryMonitor()
    {
        stop();
        std::cout << "PrimaryMonitor stopped" << std::endl;
    }

    std::thread *start()
    {
        monitorThread = std::thread(&PrimaryMonitor::run, this);
        return &monitorThread;
    }

    void stop()
    {
        messageHandler.stopMonitoring();
    }

  private:
    std::thread monitorThread;
    BackupMessageHandler &messageHandler;

    void run()
    {
        messageHandler.monitorMessages();
    }
};
} // namespace backup
