#include <vector>

namespace common
{
constexpr size_t DEFAULT_FILE_CHUNK_SIZE = 1024 * 64;

struct FileChunk
{
    FileChunk(const std::vector<char> &data, const size_t numPacket, const size_t totalPackets)
        : numPacket(numPacket), totalPackets(totalPackets), data(data)
    {
    }

    static size_t geTotalPackets(const size_t fileSize, const size_t chunkSize = DEFAULT_FILE_CHUNK_SIZE)
    {
        return (fileSize / chunkSize) + (fileSize % chunkSize != 0);
    }

    bool isLastChunk() const
    {
        return numPacket == totalPackets - 1;
    }

    const std::vector<char> &data;
    const size_t numPacket;
    const size_t totalPackets;
};
} // namespace common