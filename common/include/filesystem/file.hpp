#pragma once

#include "fileChunk.hpp"
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
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

    std::string getName() const
    {
        return getFileName(path);
    }

    std::string getExtension() const
    {
        return getFileExtension(path);
    }

    void readFile(OnChunkReadCallback callback, size_t chunkSize = DEFAULT_FILE_CHUNK_SIZE)
    {
        size_t totalPackets = FileChunk::geTotalPackets(getSize(), chunkSize);
        // index starts in 1 because packetNum starts in 1
        for (size_t i = 1; i <= totalPackets; i++)
        {
            std::vector<char> chunkData = getChunkData(chunkSize);
            FileChunk chunk(chunkData, i, totalPackets);
            callback(chunk);
        }
    }

    void writeFile(OnChunkWriteCallback callback)
    {
        std::cout << "Writing to file: " << path << std::endl;

        std::ofstream outFile(path, std::ios::binary);
        if (!outFile.is_open())
        {
            throw std::runtime_error("Failed to open file for writing: " + path);
        }

        while (true)
        {
            FileChunk chunk = callback();

            if (chunk.data.empty())
            {
                throw std::runtime_error("Chunk data is empty");
            }

            outFile.write(chunk.data.data(), chunk.data.size());

            if (!outFile.good())
            {
                throw std::runtime_error("Failed to write to file: " + path);
            }

            if (chunk.isLastChunk())
            {
                return;
            }
        }
    }

    static std::string getFileExtension(const std::string &fileName)
    {
        return std::filesystem::path(fileName).extension().string();
    }

    static std::string getFileName(const std::string &filePath)
    {
        return std::filesystem::path(filePath).filename().string();
    }

  private:
    const std::string path;
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