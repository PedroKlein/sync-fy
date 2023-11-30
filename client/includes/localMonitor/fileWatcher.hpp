#pragma once

#include <cstring>
#include <filesystem/file.hpp>
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
    FileWatcher(const FileWatcher &) = delete;
    FileWatcher &operator=(const FileWatcher &) = delete;

    ~FileWatcher();

    static FileWatcher &getInstance(const std::string &dirPath);

    void setFileAddedCallback(std::function<void(const std::string &)> callback);
    void setFileRemovedCallback(std::function<void(const std::string &)> callback);
    void setFileModifiedCallback(std::function<void(const std::string &)> callback);

    void pauseFileWatching();
    void resumeFileWatching();

    void processEvents();

  private:
    FileWatcher(const std::string &dirPath);

    static const int EVENT_SIZE = (sizeof(struct inotify_event));
    static const int EVENT_BUF_LEN = 1024;
    bool isPaused = false;

    int inotifyFd;
    int watchFd;

    std::string dirPath;

    std::function<void(const std::string &)> fileAddedCallback;
    std::function<void(const std::string &)> fileRemovedCallback;
    std::function<void(const std::string &)> fileModifiedCallback;

    std::string getFileExtension(const std::string &fileName);
};
} // namespace localMonitor
