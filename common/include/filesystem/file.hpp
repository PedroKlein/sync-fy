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
    }

    size_t getSize()
    {
        std::streampos currentPos = fileStream.tellg();
        fileStream.seekg(0, std::ios::end);
        size_t size = fileStream.tellg();
        fileStream.seekg(currentPos);
        return size;
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

    std::vector<char> getChunk(size_t chunkSize = DEFAULT_FILE_CHUNK_SIZE)
    {
        std::vector<char> buffer(chunkSize);
        fileStream.read(buffer.data(), chunkSize);
        buffer.resize(fileStream.gcount());
        return buffer;
    }
};
} // namespace common