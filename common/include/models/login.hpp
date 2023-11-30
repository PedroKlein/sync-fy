
#pragma once

#include <models/baseModel.hpp>

namespace common
{
class Login : public BaseModel
{
  public:
    std::string username;

    Login() : BaseModel(MessageType::LOGIN)
    {
    }

    Login(const std::string &username) : BaseModel(MessageType::LOGIN), username(username)
    {
    }

    std::string toJson() const override
    {
        Json::Value root;
        root["username"] = username;
        Json::StreamWriterBuilder writer;
        std::string output = Json::writeString(writer, root);
        return output;
    }

    void fromJson(const std::string &jsonStr) override
    {
        Json::Value root;
        Json::CharReaderBuilder reader;
        std::string errors;
        std::istringstream iss(jsonStr);
        if (!Json::parseFromStream(reader, iss, &root, &errors))
        {
            throw std::runtime_error("Failed to parse JSON");
        }
        username = root["username"].asString();
    }
};
} // namespace common