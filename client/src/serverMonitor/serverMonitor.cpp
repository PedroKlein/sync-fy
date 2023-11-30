#include "serverMonitor/serverMonitor.hpp"

namespace serverMonitor
{

ServerMonitor::ServerMonitor(MessageHandler &messageHandler) : messageHandler(messageHandler)
{
}

ServerMonitor::~ServerMonitor()
{
    stop();
}

std::thread *ServerMonitor::start()
{
    monitorThread = std::thread(&ServerMonitor::run, this);
    return &monitorThread;
}

void ServerMonitor::stop()
{
    messageHandler.stopMonitoring();
    if (monitorThread.joinable())
    {
        monitorThread.join();
    }
}

void ServerMonitor::run()
{
    messageHandler.monitorMessages();
}
} // namespace serverMonitor