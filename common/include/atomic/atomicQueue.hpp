#pragma once

#include <mutex>
#include <queue>

namespace common
{
/**
 * @brief Template class representing a thread-safe queue with atomic operations.
 * @tparam T The type of elements stored in the queue.
 */
template <typename T> class AtomicQueue
{
  private:
    std::queue<T> queue;
    std::mutex mtx;

  public:
    /**
     * @brief Default constructor for AtomicQueue.
     */
    AtomicQueue() = default;

    /**
     * @brief Default destructor for AtomicQueue.
     */
    ~AtomicQueue() = default;

    // Copy operations are deleted to prevent unintended behavior in a multi-threaded environment.
    AtomicQueue(const AtomicQueue &) = delete;
    AtomicQueue &operator=(const AtomicQueue &) = delete;

    /**
     * @brief Pushes an element onto the queue in a thread-safe manner.
     * @param data The element to be pushed onto the queue.
     */
    void push(const T &data)
    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(data);
    }

    /**
     * @brief Pops an element from the queue in a thread-safe manner.
     * @return The element popped from the queue.
     * @throws std::out_of_range if the queue is empty.
     */
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

    /**
     * @brief Attempts to pop an element from the queue in a thread-safe manner.
     * @param data A reference to store the popped element.
     * @return True if an element was successfully popped, false if the queue is empty.
     */
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

    /**
     * @brief Checks if the queue is empty in a thread-safe manner.
     * @return True if the queue is empty, false otherwise.
     */
    bool empty()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
};
} // namespace common