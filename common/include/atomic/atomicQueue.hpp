
#include <mutex>
#include <queue>

namespace common
{
template <typename T> class AtomicQueue
{
  private:
    std::queue<T> queue;
    std::mutex mtx;

  public:
    AtomicQueue() = default;
    ~AtomicQueue() = default;

    AtomicQueue(const AtomicQueue &) = delete;
    AtomicQueue &operator=(const AtomicQueue &) = delete;

    void push(const T &data)
    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(data);
    }

    T pop()
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (queue.empty())
        {
            throw std::out_of_range("Queue is empty");
        }
        T data = queue.front();
        queue.pop();
        return data;
    }

    bool tryPop(T &data)
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (queue.empty())
        {
            return false;
        }
        data = queue.front();
        queue.pop();
        return true;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
};
} // namespace common