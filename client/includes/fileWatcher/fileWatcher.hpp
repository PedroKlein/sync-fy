#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/inotify.h>
#include <functional>
#include <sys/stat.h>
#include <thread>

#define DEFAULT_PATH "./sync_dir"

class FileWatcher
{
private:
    static const int EVENT_SIZE = (sizeof(struct inotify_event));
    static const int EVENT_BUF_LEN = 1024;

    int inotifyFd;
    int watchFd;

    std::string dirPath;

    std::function<void(const std::string &)> fileAddedCallback;
    std::function<void(const std::string &)> fileRemovedCallback;
    std::function<void(const std::string &)> fileModifiedCallback;

    void processEvents();

public:
    FileWatcher(const char *dirPath) : dirPath(dirPath)
    {
        mkdir(dirPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

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
        close(watchFd);
        close(inotifyFd);
    }

    std::thread* start();

    void setFileAddedCallback(std::function<void(const std::string &)> callback);
    void setFileRemovedCallback(std::function<void(const std::string &)> callback);
    void setFileModifiedCallback(std::function<void(const std::string &)> callback);

    void pauseFileWatching();
    void resumeFileWatching();
};

#endif