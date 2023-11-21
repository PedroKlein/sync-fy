#pragma once

#include <iostream>
#include <string>
#include <vector>

class FileWatcher
{
  private:
    std::string syncDir;
    std::vector<std::string> files;

  public:
    FileWatcher(char *syncDir);
    ~FileWatcher();

    static void *Watch(void *arg);
};

FileWatcher::FileWatcher(char *syncDir)
{
    this->syncDir = syncDir;
}

FileWatcher::~FileWatcher()
{
}