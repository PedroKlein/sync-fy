#pragma once

#include "fileChunk.hpp"
#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <vector>
namespace common
{

using OnChunkReadCallback = std::function<void(const FileChunk &)>;

using OnChunkWriteCallback = std::function<FileChunk()>;

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

    void readFile(OnChunkReadCallback callback, size_t chunkSize = DEFAULT_FILE_CHUNK_SIZE)
    {
        size_t totalPackets = FileChunk::geTotalPackets(getSize(), chunkSize);
        for (size_t i = 0; i < totalPackets; i++)
        {
            std::vector<char> chunkData = getChunkData(chunkSize);
            FileChunk chunk(chunkData, i, totalPackets);
            callback(chunk);
        }
    }

    void writeFile(OnChunkWriteCallback callback, size_t chunkSize = DEFAULT_FILE_CHUNK_SIZE)
    {
        std::ofstream outFile(path, std::ios::binary);
        if (!outFile)
        {
            throw std::runtime_error("Failed to open file for writing: " + path);
        }

        while (true)
        {
            FileChunk chunk = callback();
            outFile.write(chunk.data.data(), chunk.data.size());

            if (chunk.isLastChunk())
            {
                break;
            }
        }
    }

  private:
    std::string path;
    std::ifstream fileStream;

    const std::vector<char> getChunkData(size_t chunkSize = DEFAULT_FILE_CHUNK_SIZE)
    {
        std::vector<char> buffer(chunkSize);
        fileStream.read(buffer.data(), chunkSize);
        buffer.resize(fileStream.gcount());
        return buffer;
    }
};
} // namespace common