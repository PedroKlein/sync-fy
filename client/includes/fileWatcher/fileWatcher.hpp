#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/inotify.h>
#include <functional>
#include <thread>
#include <filesystem>

#define DEFAULT_PATH "./sync_dir"

class FileWatcher
{
private:
    static const int EVENT_SIZE = (sizeof(struct inotify_event));
    static const int EVENT_BUF_LEN = 1024;

    int inotifyFd;
    int watchFd;

    bool isRunning = true;
    std::thread watcherThread;

    std::string dirPath;

    std::function<void(const std::string &)> fileAddedCallback;
    std::function<void(const std::string &)> fileRemovedCallback;
    std::function<void(const std::string &)> fileModifiedCallback;

    void processEvents();
    void run();

public:
    FileWatcher(const char *dirPath) : dirPath(dirPath)
    {
        if(std::filesystem::exists(dirPath)) 
        {
            try {
                std::filesystem::remove_all(dirPath);
            } catch (const std::exception& e) {
                std::cerr << "Error removing directory: " << e.what() << std::endl;
            }
        }

        try {
            std::filesystem::create_directory(dirPath);
        } catch (const std::exception& e) {
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
        stop();
    }

    std::thread* start();
    void stop();

    void setFileAddedCallback(std::function<void(const std::string &)> callback);
    void setFileRemovedCallback(std::function<void(const std::string &)> callback);
    void setFileModifiedCallback(std::function<void(const std::string &)> callback);

    void pauseFileWatching();
    void resumeFileWatching();
};

#endif