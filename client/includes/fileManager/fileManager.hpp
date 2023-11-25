#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/inotify.h>
#include <functional>

namespace filemanager {

    class FileManager
    {
    private:
        static const int EVENT_SIZE = (sizeof(struct inotify_event));
        static const int EVENT_BUF_LEN = 1024;

        int inotifyFd;
        int watchFd;

        std::string dirPath;

        std::function<void(const std::string&)> fileAddedCallback;
        std::function<void(const std::string&)> fileRemovedCallback;
        std::function<void(const std::string&)> fileModifiedCallback;

        void processEvents();
        
    public:
        FileManager(const std::string& dirPath) : dirPath(dirPath)
        {
            inotifyFd = inotify_init();
            if (inotifyFd < 0)
            {
                std::cout << "inotify_init failed" << std::endl;
                exit(EXIT_FAILURE);
            }

            watchFd = inotify_add_watch(inotifyFd, dirPath.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
            if (watchFd < 0)
            {
                std::cout << "inotify_add_watch failed" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        ~FileManager()
        {
            close(watchFd);
            close(inotifyFd);
        }

        void startMonitoring();

        void setFileAddedCallback(std::function<void(const std::string&)> callback);
        void setFileRemovedCallback(std::function<void(const std::string&)> callback);
        void setFileModifiedCallback(std::function<void(const std::string&)> callback);
    };

}
#endif