#pragma once

#include <cstring>
#include <fcntl.h>
#include <filesystem/file.hpp>
#include <filesystem>
#include <functional>
#include <iostream>
#include <sys/inotify.h>
#include <thread>
#include <unistd.h>

namespace localMonitor
{

/**
 * @class FileWatcher
 * @brief Singleton class that watches for file changes in a directory using inotify.
 */
class FileWatcher
{
  public:
    /**
     * @brief Deleted copy constructor to prevent unintended copies.
     */
    FileWatcher(const FileWatcher &) = delete;
    
    /**
     * @brief Deleted copy assignment operator to prevent unintended copies.
     */
    FileWatcher &operator=(const FileWatcher &) = delete;

    /**
     * @brief Destructor for FileWatcher.
     */
    ~FileWatcher();

    /**
     * @brief Gets the singleton instance of FileWatcher for the specified directory path.
     * @param dirPath The path of the directory to monitor.
     * @return Reference to the singleton instance of FileWatcher.
     */
    static FileWatcher &getInstance(const std::string &dirPath);

    /**
     * @brief Sets the callback for the "file added" event.
     * @param callback The callback function to be invoked when a file is added.
     */
    void setFileAddedCallback(std::function<void(const std::string &)> callback);
    
    /**
     * @brief Sets the callback for the "file removed" event.
     * @param callback The callback function to be invoked when a file is removed.
     */
    void setFileRemovedCallback(std::function<void(const std::string &)> callback);

    /**
     * @brief Sets the callback for the "file modified" event.
     * @param callback The callback function to be invoked when a file is modified.
     */
    void setFileModifiedCallback(std::function<void(const std::string &)> callback);

    /**
     * @brief Pauses file watching, events will not be processed.
     */
    void pauseFileWatching();

    /**
     * @brief Resumes file watching, allowing events to be processed.
     */
    void resumeFileWatching();

    /**
     * @brief Processes inotify events and invokes corresponding callbacks.
     */
    void processEvents();

  private:
    /**
     * @brief Private constructor for FileWatcher.
     * @param dirPath The path of the directory to monitor.
     */
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

    /**
     * @brief Gets the file extension from the given file name.
     * @param fileName The name of the file.
     * @return The file extension.
     */
    std::string getFileExtension(const std::string &fileName);
};
} // namespace localMonitor
