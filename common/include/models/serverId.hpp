#pragma once

#include <models/baseModel.hpp>

namespace common
{
class ServerId : public BaseModel
{
  public:
    int serverId;

    ServerId() : BaseModel(MessageType::DELETE_FILE)
    {
    }

    ServerId(const int &serverId) : BaseModel(MessageType::SERVER_ID), serverId(serverId)
    {
    }

    std::string toJson() const override
    {
        Json::Value root;
        root["serverId"] = static_cast<Json::Int64>(serverId);
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
        serverId = root["serverId"].asInt();
    }
};
} // namespace common