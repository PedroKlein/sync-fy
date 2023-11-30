#pragma once

#include <sstream>
#include <string>

namespace common
{
class DateTime
{
  public:
    static std::string toLocalTime(time_t t)
    {
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y-%m-%d %X");
        return ss.str();
    }
};
} // namespace common
