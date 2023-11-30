#pragma once

namespace common
{
class InitSendFile : public BaseModel
{
  public:
    std::string filename;
    size_t fileSize;

    InitSendFile() : BaseModel(MessageType::INIT_SEND_FILE)
    {
    }

    InitSendFile(const std::string &filename, size_t fileSize)
        : BaseModel(MessageType::INIT_SEND_FILE), filename(filename), fileSize(fileSize)
    {
    }

    std::string toJson() const override
    {
        Json::Value root;
        root["filename"] = filename;
        root["fileSize"] = static_cast<Json::UInt64>(fileSize);
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
        fileSize = root["fileSize"].asUInt();
    }
};
} // namespace common