#include <fstream>
#include <string>
#include <vector>

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

    std::vector<char> getChunk(size_t chunkSize)
    {
        std::vector<char> buffer(chunkSize);
        fileStream.read(buffer.data(), chunkSize);
        buffer.resize(fileStream.gcount());
        return buffer;
    }

  private:
    std::string path;
    std::ifstream fileStream;
};