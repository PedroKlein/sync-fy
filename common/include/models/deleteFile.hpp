#pragma once

namespace common
{
class DeleteFile : public BaseModel
{
  public:
    std::string filename;

    DeleteFile() : BaseModel(MessageType::DELETE_FILE) {}

    DeleteFile(const std::string &filename) : BaseModel(MessageType::DELETE_FILE), filename(filename) {}

    std::string toJson() const override
    {
        Json::Value root;
        root["filename"] = filename;
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
        filename = root["filename"].asString();
    }
};
} // namespace common