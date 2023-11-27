#include "localMonitor/fileWatcher.hpp"

namespace localMonitor
{

void FileWatcher::setFileAddedCallback(std::function<void(const std::string &)> callback)
{
    fileAddedCallback = std::move(callback);
}

void FileWatcher::setFileRemovedCallback(std::function<void(const std::string &)> callback)
{
    fileRemovedCallback = std::move(callback);
}

void FileWatcher::setFileModifiedCallback(std::function<void(const std::string &)> callback)
{
    fileModifiedCallback = std::move(callback);
}

void FileWatcher::processEvents()
{
    char buffer[EVENT_BUF_LEN];
    int length = read(inotifyFd, buffer, EVENT_BUF_LEN);

    if (length < 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < length;)
    {
        struct inotify_event *event = reinterpret_cast<struct inotify_event *>(&buffer[i]);
        std::string fileName = event->name ? event->name : "";
        std::string filePath = dirPath + "/" + fileName;

        if (event->mask & IN_MODIFY)
        {
            if (fileModifiedCallback)
            {
                fileModifiedCallback(filePath);
            }
        }
        else if (event->mask & IN_CREATE)
        {
            if (fileAddedCallback)
            {
                fileAddedCallback(filePath);
            }
        }
        else if (event->mask & IN_DELETE)
        {
            if (fileRemovedCallback)
            {
                fileRemovedCallback(filePath);
            }
        }

        i += EVENT_SIZE + event->len;
    }
}
} // namespace localMonitor