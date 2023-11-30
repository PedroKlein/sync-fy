#pragma once

#include <ctime>
#include <filesystem>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

namespace common
{
struct FileInfo
{
    std::string filename;
    off_t filesize;
    time_t modificationTime;
    time_t accessTime;
    time_t creationTime;
};

class Directory
{
  public:
    explicit Directory(const std::string &path) : path_(path)
    {
    }

    std::vector<FileInfo> listFiles() const
    {
        std::vector<FileInfo> files;

        for (const auto &entry : std::filesystem::directory_iterator(path_))
        {
            FileInfo fileInfo = getMACtimes(entry.path().string());
            files.push_back(fileInfo);
        }

        return files;
    }

  private:
    std::string path_;

    FileInfo getMACtimes(const std::string &filepath) const
    {
        FileInfo fileInfo;

        struct stat fileStat;
        if (stat(filepath.c_str(), &fileStat) != 0)
        {
            throw std::runtime_error("Error getting file information");
        }

        fileInfo.filename = File::getFileName(filepath);
        fileInfo.filesize = fileStat.st_size;
        fileInfo.modificationTime = fileStat.st_mtime;
        fileInfo.accessTime = fileStat.st_atime;
        fileInfo.creationTime = fileStat.st_ctime;

        return fileInfo;
    }
};
} // namespace common
