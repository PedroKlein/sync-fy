#pragma once

#include "clientMessageHandler.hpp"
#include "fileWatcher.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <thread>

namespace localMonitor
{

class LocalMonitor
{
  public:
    LocalMonitor(FileWatcher &fileWatcher, const ClientMessageHandler &messageHandler);
    ~LocalMonitor();

    std::thread *start();
    void stop();

  private:
    std::thread monitorThread;
    bool isRunning;
    FileWatcher &fileWatcher;
    const ClientMessageHandler &messageHandler;

    void run();
    void onFileAddedOrModified(const std::string &filePath);
    void onFileRemoved(const std::string &filePath);
};
} // namespace localMonitor
