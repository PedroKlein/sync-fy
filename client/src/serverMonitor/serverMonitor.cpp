#include "serverMonitor/serverMonitor.hpp"

namespace serverMonitor
{

ServerMonitor::ServerMonitor(MessageHandler &messageHandler) : messageHandler(messageHandler)
{
}

ServerMonitor::~ServerMonitor()
{
    stop();
    std::cout << "ServerMonitor stopped" << std::endl;
}

std::thread *ServerMonitor::start()
{
    monitorThread = std::thread(&ServerMonitor::run, this);
    return &monitorThread;
}

void ServerMonitor::stop()
{
    messageHandler.stopMonitoring();
}

void ServerMonitor::run()
{
    messageHandler.monitorMessages();
}
} // namespace serverMonitor