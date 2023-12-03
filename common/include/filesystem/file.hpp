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

/**
 * @class File
 * @brief Represents a file and provides methods for reading and writing file chunks.
 *
 * The File class facilitates reading and writing file data in chunks, as well as
 * providing information about the file, such as its size, name, and extension.
 */
class File
{
  public:
    /**
     * @brief Constructs a File object with the specified file path.
     *
     * @param filePath The path to the file.
     * @throws std::runtime_error if the file does not exist.
     */
    File(const std::string &filePath) : path(filePath), fileStream(filePath, std::ios::binary)
    {
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error("File does not exist: " + filePath);
        }
    }

    /**
     * @brief Creates a new file with the specified file path.
     *
     * @param filePath The path to the file to be created.
     * @return A File object representing the newly created file.
     * @throws std::runtime_error if the file creation fails.
     */
    static File create(const std::string &filePath)
    {
        std::ofstream ofs(filePath, std::ios::binary);
        if (!ofs)
        {
            throw std::runtime_error("Failed to create file: " + filePath);
        }
        return File(filePath);
    }

    /**
     * @brief Gets the size of the file in bytes.
     *
     * @return The size of the file in bytes.
     */
    const size_t getSize() const
    {
        return std::filesystem::file_size(path);
    }

    /**
     * @brief Gets the name of the file.
     *
     * @return The name of the file.
     */
    std::string getName() const
    {
        return getFileName(path);
    }

    /**
     * @brief Gets the extension of the file.
     *
     * @return The extension of the file.
     */
    std::string getExtension() const
    {
        return getFileExtension(path);
    }

    /**
     * @brief Reads the file in chunks and invokes a callback for each chunk.
     *
     * @param callback The callback function to be invoked for each file chunk.
     * @param chunkSize The size of each file chunk in bytes.
     */
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

    /**
     * @brief Writes the file using a callback function to provide each file chunk.
     *
     * @param callback The callback function providing each file chunk to be written.
     * @throws std::runtime_error if writing to the file fails.
     */
    void writeFile(OnChunkWriteCallback callback)
    {
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

    /**
     * @brief Gets the file extension from the provided file name or path.
     *
     * @param fileName The file name or path.
     * @return The file extension.
     */
    static std::string getFileExtension(const std::string &fileName)
    {
        return std::filesystem::path(fileName).extension().string();
    }

    /**
     * @brief Gets the file name from the provided file path.
     *
     * @param filePath The file path.
     * @return The file name.
     */
    static std::string getFileName(const std::string &filePath)
    {
        return std::filesystem::path(filePath).filename().string();
    }

  private:
    const std::string path;
    std::ifstream fileStream;

    /**
     * @brief Reads data from the file stream to create a file chunk.
     *
     * @param chunkSize The size of the file chunk to be read in bytes.
     * @return A vector of characters representing the file chunk data.
     */
    const std::vector<char> getChunkData(size_t chunkSize = DEFAULT_FILE_CHUNK_SIZE)
    {
        std::vector<char> buffer(chunkSize);
        fileStream.read(buffer.data(), chunkSize);
        buffer.resize(fileStream.gcount());
        return buffer;
    }
};
} // namespace common