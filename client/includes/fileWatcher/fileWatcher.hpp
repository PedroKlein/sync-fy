#ifndef FILEWATCHER_HPP
#define FILEWATCHER_HPP

#include <iostream>
#include <vector>
#include <string>

class FileWatcher
{
private:
    std::string syncDir;
    std::vector<std::string> files;
public:
    FileWatcher(char* syncDir);
    ~FileWatcher();

    static void* Watch(void* arg);
};

FileWatcher::FileWatcher(char* syncDir)
{
    this->syncDir = syncDir;
}

FileWatcher::~FileWatcher()
{
}






#endif