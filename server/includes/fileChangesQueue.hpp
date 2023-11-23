
#include <mutex>
#include <queue>

enum FileChangesType
{
    FILE_CREATED,
    FILE_DELETED,
};

struct FileChanges
{
    std::string filename;
    FileChangesType changeType;
};

class FileChangesQueue
{
  private:
    std::queue<FileChanges> queue;
    std::mutex mtx;

  public:
    FileChangesQueue() = default;
    ~FileChangesQueue() = default;

    FileChangesQueue(const FileChangesQueue &) = delete;
    FileChangesQueue &operator=(const FileChangesQueue &) = delete;

    void push(const FileChanges &fileChanges)
    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(fileChanges);
    }

    FileChanges pop()
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (queue.empty())
        {
            throw std::out_of_range("Queue is empty");
        }
        FileChanges fileChanges = queue.front();
        queue.pop();
        return fileChanges;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
};