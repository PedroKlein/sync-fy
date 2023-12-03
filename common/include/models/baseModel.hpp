#pragma once

#include "messages/messageTypes.hpp"
#include <json/json.h>
#include <sstream>
#include <string>

namespace common
{
class BaseModel
{
  public:
    BaseModel(MessageType type) : type(type)
    {
    }

    virtual ~BaseModel()
    {
    }

    virtual std::string toJson() const = 0;
    virtual void fromJson(const std::string &jsonStr) = 0;

    const MessageType getType() const
    {
        return type;
    }

  private:
    const MessageType type;
};
} // namespace common