#include "localMonitor/localMonitor.hpp"

namespace localMonitor
{

LocalMonitor::LocalMonitor(FileWatcher &fileWatcher, const MessageHandler &messageHandler)
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
    if (monitorThread.joinable())
    {
        monitorThread.join();
    }
}

void LocalMonitor::run()
{
    while (isRunning)
    {
        fileWatcher.processEvents();
    }
}

void LocalMonitor::onFileAddedOrModified(const std::string &filePath)
{
    std::cout << "File added or modified: " << filePath << std::endl;
    common::File file(filePath);

    messageHandler.sendInitUploadFileMessage(file.getName(), file.getSize());

    messageHandler.sendFileMessage(file);

    messageHandler.receiveOK();
}

void LocalMonitor::onFileRemoved(const std::string &filePath)
{
    std::cout << "File removed: " << filePath << std::endl;
    messageHandler.sendDeleteFileMessage(filePath);
}

} // namespace localMonitor