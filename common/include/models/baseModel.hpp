#pragma once

#include <jsoncpp/json/json.h>

class BaseModel
{
  public:
    virtual ~BaseModel()
    {
    }

    virtual std::string toJson() const = 0;
    virtual void fromJson(const std::string &jsonStr) = 0;
};