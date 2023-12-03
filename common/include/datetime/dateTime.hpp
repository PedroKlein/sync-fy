#pragma once

#include <sstream>
#include <string>

namespace common
{
/**
 * @class DateTime
 * @brief Provides functionality for working with date and time.
 *
 * The DateTime class contains static methods for converting time_t values
 * to local time representations in the format "%Y-%m-%d %X".
 */
class DateTime
{
  public:
    /**
     * @brief Converts a time_t value to a local time string.
     *
     * Converts the given time_t value to a string representing the local time
     * in the format "%Y-%m-%d %X".
     *
     * @param t The time_t value to be converted.
     * @return A string representing the local time in the specified format.
     */
    static std::string toLocalTime(time_t t)
    {
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y-%m-%d %X");
        return ss.str();
    }
};
} // namespace common
