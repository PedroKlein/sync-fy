#pragma once

namespace common
{

// This obviusly does not scale well, but it is simple
class ConnectedIps : public BaseModel
{
  public:
    std::vector<std::string> ips;

    ConnectedIps() : BaseModel(MessageType::CONNECTED_IPS)
    {
    }

    ConnectedIps(const std::vector<std::string> &ips) : BaseModel(MessageType::DELETE_FILE), ips(ips)
    {
    }

    std::string toJson() const override
    {
        Json::Value root;
        for (const auto &ip : ips)
        {
            root["ips"].append(ip);
        }
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
        const auto &ipsJson = root["ips"];
        if (ipsJson.isArray())
        {
            for (const auto &ipJson : ipsJson)
            {
                ips.push_back(ipJson.asString());
            }
        }
    }
};
} // namespace common