#include <vector>

namespace common
{
/** @brief The default size of a file chunk in bytes. */
constexpr size_t DEFAULT_FILE_CHUNK_SIZE = 1024 * 64;

/**
 * @struct FileChunk
 * @brief Represents a chunk of data from a file.
 *
 * The FileChunk structure holds a portion of file data, along with information
 * about the packet number, total number of packets, and whether it is the last chunk.
 */
struct FileChunk
{
    /**
     * @brief Constructs a FileChunk object with the provided data, packet number, and total number of packets.
     *
     * @param data The vector of characters representing the file data.
     * @param numPacket The packet number of this chunk.
     * @param totalPackets The total number of packets in which the file is divided.
     */
    FileChunk(const std::vector<char> data, const size_t numPacket, const size_t totalPackets)
        : numPacket(numPacket), totalPackets(totalPackets), data(data)
    {
    }

    /**
     * @brief Calculates the total number of packets needed to transmit a file of a given size with a specified chunk size.
     *
     * @param fileSize The size of the file in bytes.
     * @param chunkSize The size of each file chunk in bytes.
     * @return The total number of packets required to transmit the file.
     */
    static size_t geTotalPackets(const size_t fileSize, const size_t chunkSize = DEFAULT_FILE_CHUNK_SIZE)
    {
        return (fileSize / chunkSize) + (fileSize % chunkSize != 0);
    }

    /**
     * @brief Checks whether the current chunk is the last one in the sequence.
     *
     * @return True if the chunk is the last one, false otherwise.
     */
    bool isLastChunk() const
    {
        return numPacket == totalPackets;
    }

    const std::vector<char> data;
    const size_t numPacket;
    const size_t totalPackets;
};
} // namespace common