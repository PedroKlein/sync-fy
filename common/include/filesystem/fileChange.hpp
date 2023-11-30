
#pragma once

namespace common
{
enum FileChangeType
{
    FILE_CREATED,
    FILE_DELETED,
};

struct FileChange
{
    std::string filename;
    FileChangeType changeType;

    FileChange() = default;

    FileChange(std::string filename, FileChangeType changeType) : filename(filename), changeType(changeType)
    {
    }
};
} // namespace common