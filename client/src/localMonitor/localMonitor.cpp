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
    } while (isRunning);
}

void LocalMonitor::onFileAddedOrModified(const std::string &filePath)
{
    common::File file(filePath);
    messageHandler.sendFileMessage(file);
}

void LocalMonitor::onFileRemoved(const std::string &filePath)
{
    messageHandler.sendDeleteFileMessage(common::File::getFileName(filePath));
}

} // namespace localMonitor