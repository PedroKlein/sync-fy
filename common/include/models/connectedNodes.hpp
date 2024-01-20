#pragma once

namespace common
{

struct Node
{
    std::string ip;
    int id;

    Node()
    {
    }

    Node(std::string ip, int id) : ip(ip), id(id)
    {
    }

    bool operator==(const Node &other) const
    {
        return ip == other.ip;
    }
};
// This obviusly does not scale well, but it is simple
class ConnectedNodes : public BaseModel
{
  public:
    std::vector<Node> nodes;

    ConnectedNodes() : BaseModel(MessageType::CONNECTED_NODES)
    {
    }

    ConnectedNodes(const std::vector<Node> &nodes) : BaseModel(MessageType::CONNECTED_NODES), nodes(nodes)
    {
    }

    std::string toJson() const override
    {
        Json::Value root;
        for (const auto &node : nodes)
        {
            Json::Value nodeJson;
            nodeJson["ip"] = node.ip;
            nodeJson["id"] = node.id;
            root["nodes"].append(nodeJson);
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
        const auto &nodesJson = root["nodes"];
        if (nodesJson.isArray())
        {
            for (const auto &nodeJson : nodesJson)
            {
                Node node;
                node.ip = nodeJson["ip"].asString();
                node.id = nodeJson["id"].asInt();
                nodes.push_back(node);
            }
        }
    }
};
} // namespace common