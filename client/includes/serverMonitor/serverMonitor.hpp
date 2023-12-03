#pragma once

#include "serverMonitor/messageHandler.hpp"
#include <iostream>
#include <string>
#include <thread>

namespace serverMonitor
{
/**
 * @class ServerMonitor
 * @brief Monitors server-specific events and handles messages using a MessageHandler.
 * 
 * This class is responsible for monitoring server-specific events and handling messages
 * using the provided MessageHandler.
 */
class ServerMonitor
{
  public:
    /**
     * @brief Constructor for ServerMonitor.
     * @param messageHandler The MessageHandler to handle server-specific messages.
     */
    ServerMonitor(MessageHandler &messageHandler);

    /**
     * @brief Destructor for ServerMonitor.
     */
    ~ServerMonitor();

    /**
     * @brief Starts the server monitoring in a separate thread.
     * @return A pointer to the thread handling the server monitoring.
     */
    std::thread *start();

    /**
     * @brief Stops the server monitoring.
     */
    void stop();

  private:
    std::thread monitorThread;
    MessageHandler &messageHandler;

    /**
     * @brief Internal function to run the server monitoring in a separate thread.
     */
    void run();
};
} // namespace serverMonitor
