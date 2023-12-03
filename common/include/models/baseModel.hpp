#pragma once

#include "messages/messageTypes.hpp"
#include <json/json.h>
#include <sstream>
#include <string>

namespace common
{
/**
* @class BaseModel
* @brief The abstract base class for models in the communication protocol.
*/
class BaseModel
{
  public:
    /**
    * @brief Constructs a BaseModel with the specified MessageType.
    * @param type The MessageType associated with the BaseModel.
    */
    BaseModel(MessageType type) : type(type)
    {
    }

    /**
    * @brief Virtual destructor to ensure proper cleanup in derived classes.
    */
    virtual ~BaseModel()
    {
    }

    /**
    * @brief Converts the BaseModel to a JSON-formatted string.
    * @return The JSON-formatted string representing the BaseModel.
    */
    virtual std::string toJson() const = 0;

    /**
    * @brief Converts the BaseModel to a JSON-formatted string.
    * @return The JSON-formatted string representing the BaseModel.
    */
    virtual void fromJson(const std::string &jsonStr) = 0;

    const MessageType getType() const
    {
        return type;
    }

  private:
    const MessageType type;
};
} // namespace common