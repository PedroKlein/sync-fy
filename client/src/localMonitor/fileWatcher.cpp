#include "localMonitor/fileWatcher.hpp"

namespace localMonitor
{

FileWatcher::FileWatcher(const char *dirPath) : dirPath(dirPath)
{
    if (std::filesystem::exists(dirPath))
    {
        try
        {
            std::filesystem::remove_all(dirPath);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error removing directory: " << e.what() << std::endl;
        }
    }

    try
    {
        std::cout << "Creating directory: " << dirPath << std::endl;
        std::filesystem::create_directory(dirPath);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
    }

    inotifyFd = inotify_init();
    if (inotifyFd < 0)
    {
        std::cout << "inotify_init failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    watchFd = inotify_add_watch(inotifyFd, dirPath, IN_MODIFY | IN_CREATE | IN_DELETE);
    if (watchFd < 0)
    {
        std::cout << "inotify_add_watch failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

FileWatcher::~FileWatcher()
{
    inotify_rm_watch(inotifyFd, watchFd);
    close(inotifyFd);
}

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
        else if (event->mask & IN_MOVE)
        {
            // TODO: Handle move file - delete when move out of dir and create when move in dir
            if (fileRemovedCallback)
            {
                fileRemovedCallback(filePath);
            }
        }

        // TODO: handle file rename

        i += EVENT_SIZE + event->len;
    }
}
} // namespace localMonitor