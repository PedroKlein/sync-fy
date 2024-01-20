#pragma once

#include <atomic>
#include <vector>

namespace common
{
template <typename T> class AtomicVector
{
  public:
    void emplaceBack(const T &value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        vector_.emplace_back(value);
    }

    void remove(const T &value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        vector_.erase(std::remove(vector_.begin(), vector_.end(), value), vector_.end());
    }

    T &operator[](size_t index)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return vector_[index];
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return vector_.size();
    }

    template <typename Func> void forEach(Func func)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto &value : vector_)
        {
            func(value);
        }
    }

  private:
    std::vector<T> vector_;
    std::mutex mutex_;
};
} // namespace common