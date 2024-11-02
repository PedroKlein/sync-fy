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
/**
 * @struct FileInfo
 * @brief Represents information about a file in a directory.
 */
struct FileInfo
{
    std::string filename;
    off_t filesize;
    time_t modificationTime;
    time_t accessTime;
    time_t creationTime;
};

/**
 * @class Directory
 * @brief Represents a directory and provides methods to interact with its files.
 */
class Directory
{
  public:
    /**
     * @brief Constructs a Directory object with the specified path.
     * @param path The path to the directory.
     */
    explicit Directory(const std::string &path) : path_(path)
    {
    }

    /**
     * @brief Lists information about files in the directory.
     * @return A vector of FileInfo objects representing the files in the directory.
     */
    std::vector<FileInfo> listFiles() const
    {
        std::vector<FileInfo> files;

        for (const auto &entry : std::filesystem::directory_iterator(path_))
        {
            if (entry.is_directory())
            {
                continue;
            }
            FileInfo fileInfo = getMACtimes(entry.path().string());
            files.push_back(fileInfo);
        }

        return files;
    }

    /**
     * @brief Returns the name of the directory.
     * @return The name of the directory.
     */
    std::string getDirectoryName() const
    {
        size_t lastSlashPos = path_.find_last_of('/');
        if (lastSlashPos != std::string::npos)
        {
            return path_.substr(lastSlashPos + 1);
        }
        return path_;
    }

  private:
    std::string path_;

    /**
     * @brief Retrieves the MAC (Modification, Access, and Creation) times of a file.
     * @param filepath The path to the file.
     * @return A FileInfo object containing information about the file.
     * @throw std::runtime_error if there is an error getting file information.
     */
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
