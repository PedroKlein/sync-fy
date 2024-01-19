#pragma once

#include "backupMessageHandler.hpp"
#include <iostream>
#include <string>
#include <thread>

namespace backup
{
class PrimaryDataMonitor
{
  public:
    PrimaryDataMonitor(BackupMessageHandler &messageHandler) : messageHandler(messageHandler)
    {
    }

    ~PrimaryDataMonitor()
    {
        stop();
        std::cout << "PrimaryDataMonitor stopped" << std::endl;
    }

    std::thread *start()
    {
        monitorThread = std::thread([this]() { run(); });
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
