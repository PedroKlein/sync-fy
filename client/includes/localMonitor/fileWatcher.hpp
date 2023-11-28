#pragma once

#include <cstring>
#include <filesystem>
#include <functional>
#include <iostream>
#include <sys/inotify.h>
#include <thread>
#include <unistd.h>

namespace localMonitor
{

class FileWatcher
{
  public:
    FileWatcher(const char *dirPath) : dirPath(dirPath)
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

    ~FileWatcher()
    {
        inotify_rm_watch(inotifyFd, watchFd);
        close(inotifyFd);
    }

    void setFileAddedCallback(std::function<void(const std::string &)> callback);
    void setFileRemovedCallback(std::function<void(const std::string &)> callback);
    void setFileModifiedCallback(std::function<void(const std::string &)> callback);

    void pauseFileWatching();
    void resumeFileWatching();

    void processEvents();

  private:
    static const int EVENT_SIZE = (sizeof(struct inotify_event));
    static const int EVENT_BUF_LEN = 1024;

    int inotifyFd;
    int watchFd;

    std::string dirPath;

    std::function<void(const std::string &)> fileAddedCallback;
    std::function<void(const std::string &)> fileRemovedCallback;
    std::function<void(const std::string &)> fileModifiedCallback;
};
} // namespace localMonitor