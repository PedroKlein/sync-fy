#include "localMonitor/fileWatcher.hpp"

namespace localMonitor
{

    FileWatcher::FileWatcher(const std::string &dirPath) : dirPath(dirPath)
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

        watchFd = inotify_add_watch(inotifyFd, dirPath.data(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_MOVE);
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

    FileWatcher &FileWatcher::getInstance(const std::string &dirPath)
    {
        static FileWatcher instance(dirPath);
        return instance;
    }

    void FileWatcher::pauseFileWatching()
    {
        isPaused = true;
    }

    void FileWatcher::resumeFileWatching()
    {
        isPaused = false;
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

        // Set the file descriptor to non-blocking
        int flags = fcntl(inotifyFd, F_GETFL, 0);
        fcntl(inotifyFd, F_SETFL, flags | O_NONBLOCK);

        int length = read(inotifyFd, buffer, EVENT_BUF_LEN);

        if (length < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // The read would have blocked, so there's no data available
                return;
            }
            else
            {
                perror("read");
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < length;)
        {
            struct inotify_event *event = reinterpret_cast<struct inotify_event *>(&buffer[i]);

            std::string fileName = event->name ? event->name : "";
            std::string fileExtension = common::File::getFileExtension(fileName);

            bool isSwap = (fileExtension == ".swx" || fileExtension == ".swp" || fileName[0] == '.'); 

            // Check if the file extension matches ".swx" or ".swp"
            if (!fileExtension.empty() && isSwap)
            {
                // Ignore this file and move to the next event
                i += EVENT_SIZE + event->len;
                std::cout << "Ignoring file: " << fileName << std::endl;
                continue;
            }

            // TODO: temporary fix, check if this has problems with concurrency
            if (isPaused)
            {
                return;
            }

            std::string filePath = dirPath + fileName;

            if (event->mask & IN_MODIFY)
            {
                if (fileModifiedCallback)
                {
                    fileModifiedCallback(filePath);
                }
            }
            else if (event->mask & IN_CREATE || event->mask & IN_MOVED_TO)
            {
                if (fileAddedCallback)
                {
                    fileAddedCallback(filePath);
                }
            }
            else if (event->mask & IN_DELETE || event->mask & IN_MOVED_FROM)
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