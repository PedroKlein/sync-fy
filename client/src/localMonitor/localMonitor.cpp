#include "localMonitor/localMonitor.hpp"

namespace localMonitor
{

LocalMonitor::LocalMonitor(FileWatcher &fileWatcher, const ClientMessageHandler &messageHandler)
    : fileWatcher(fileWatcher), messageHandler(messageHandler), isRunning(false)
{
    fileWatcher.setFileAddedCallback(std::bind(&LocalMonitor::onFileAddedOrModified, this, std::placeholders::_1));
    fileWatcher.setFileModifiedCallback(std::bind(&LocalMonitor::onFileAddedOrModified, this, std::placeholders::_1));
    fileWatcher.setFileRemovedCallback(std::bind(&LocalMonitor::onFileRemoved, this, std::placeholders::_1));
}

LocalMonitor::~LocalMonitor()
{
    stop();
    std::cout << "LocalMonitor stopped" << std::endl;
}

std::thread *LocalMonitor::start()
{
    isRunning = true;
    monitorThread = std::thread(&LocalMonitor::run, this);
    return &monitorThread;
}

void LocalMonitor::stop()
{
    isRunning = false;
}

void LocalMonitor::run()
{
    do
    {
        fileWatcher.processEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (isRunning);
}

void LocalMonitor::onFileAddedOrModified(const std::string &filePath)
{
    try
    {
        common::File file(filePath);
        messageHandler.sendFileMessage(file);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void LocalMonitor::onFileRemoved(const std::string &filePath)
{
    messageHandler.sendDeleteFileMessage(common::File::getFileName(filePath));
}

} // namespace localMonitor