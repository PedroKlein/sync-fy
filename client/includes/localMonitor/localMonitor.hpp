#pragma once

#include "clientMessageHandler.hpp"
#include "fileWatcher.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <thread>

namespace localMonitor
{

/**
 * @class LocalMonitor
 * @brief Class that monitors a directory for file changes and handles corresponding events.
 */
class LocalMonitor
{
  public:
    /**
     * @brief Constructor for LocalMonitor.
     * @param fileWatcher The FileWatcher responsible for monitoring file changes.
     * @param messageHandler The ClientMessageHandler for handling messages related to file changes.
     */
    LocalMonitor(FileWatcher &fileWatcher, const ClientMessageHandler &messageHandler);

    /**
     * @brief Destructor for LocalMonitor.
     */
    ~LocalMonitor();

    /**
     * @brief Starts the local monitoring in a separate thread.
     * @return A pointer to the thread handling the local monitoring.
     */
    std::thread *start();

    /**
     * @brief Stops the local monitoring and joins the monitoring thread.
     */
    void stop();

  private:
    std::thread monitorThread;
    bool isRunning;
    FileWatcher &fileWatcher;
    const ClientMessageHandler &messageHandler;

    /**
     * @brief Internal function to run the local monitoring in a separate thread.
     */
    void run();

    /**
     * @brief Callback function for handling file added or modified events.
     * @param filePath The path of the added or modified file.
     */
    void onFileAddedOrModified(const std::string &filePath);

    /**
     * @brief Callback function for handling file removed events.
     * @param filePath The path of the removed file.
     */
    void onFileRemoved(const std::string &filePath);
};
} // namespace localMonitor
