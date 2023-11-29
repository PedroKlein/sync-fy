
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
};
} // namespace common