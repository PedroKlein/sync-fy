
#pragma once

namespace common
{
/**
 * @enum FileChangeType
 * @brief Enumeration representing the type of file change.
 */
enum FileChangeType
{
    FILE_CREATED,
    FILE_DELETED,
};

/**
 * @struct FileChange
 * @brief Structure representing a file change.
 */
struct FileChange
{
    std::string filename;
    FileChangeType changeType;

    /**
     * @brief Default constructor for FileChange.
     */
    FileChange() = default;

    /**
     * @brief Parameterized constructor for FileChange.
     * @param filename The name of the file that changed.
     * @param changeType The type of change that occurred (created or deleted).
     */
    FileChange(std::string filename, FileChangeType changeType) : filename(filename), changeType(changeType)
    {
    }
};
} // namespace common