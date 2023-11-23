#pragma once

#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <vector>

namespace common
{
using OnChunkReadCallback = std::function<void(const std::vector<char> &)>;

using OnChunkWriteCallback = std::function<std::vector<char>()>;

constexpr size_t DEFAULT_FILE_CHUNK_SIZE = 1024 * 64;

class File
{
  public:
    File(const std::string &filePath) : path(filePath), fileStream(filePath, std::ios::binary)
    {
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error("File does not exist: " + filePath);
        }
    }

    static File create(const std::string &filePath)
    {
        std::ofstream ofs(filePath, std::ios::binary);
        if (!ofs)
        {
            throw std::runtime_error("Failed to create file: " + filePath);
        }
        return File(filePath);
    }

    const size_t getSize() const
    {
        return std::filesystem::file_size(path);
    }

    std::string getName()
    {
        size_t pos = path.find_last_of("/\\");
        return (pos == std::string::npos) ? path : path.substr(pos + 1);
    }

    void sendFile(OnChunkReadCallback callback, size_t chunkSize = DEFAULT_FILE_CHUNK_SIZE)
    {
        while (!fileStream.eof())
        {
            std::vector<char> chunk = getChunk(chunkSize);
            callback(chunk);
        }
    }

    void receiveFile(OnChunkWriteCallback callback, size_t chunkSize = DEFAULT_FILE_CHUNK_SIZE)
    {
        std::ofstream outFile(path, std::ios::binary);
        while (true)
        {
            std::vector<char> chunk = callback();
            if (chunk.empty())
            {
                break;
            }
            outFile.write(chunk.data(), chunk.size());
        }
    }

  private:
    std::string path;
    std::ifstream fileStream;

    const std::vector<char> getChunk(size_t chunkSize = DEFAULT_FILE_CHUNK_SIZE)
    {
        std::vector<char> buffer(chunkSize);
        fileStream.read(buffer.data(), chunkSize);
        buffer.resize(fileStream.gcount());
        return buffer;
    }
};
} // namespace common