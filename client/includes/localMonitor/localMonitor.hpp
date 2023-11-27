#pragma once

#include "fileWatcher.hpp"
#include "messageHandler.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <thread>

namespace localMonitor
{

class LocalMonitor
{
  public:
    LocalMonitor(FileWatcher &fileWatcher, const MessageHandler &messageHandler);
    ~LocalMonitor();

    std::thread *start();
    void stop();

  private:
    std::thread monitorThread;
    bool isRunning;
    FileWatcher &fileWatcher;
    const MessageHandler &messageHandler;

    void run();
    void onFileAddedOrModified(const std::string &filePath);
    void onFileRemoved(const std::string &filePath);
};
} // namespace localMonitor
