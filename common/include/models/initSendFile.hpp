#pragma once

namespace common
{
/**
 * @class InitSendFile
 * @brief Represents a request to initialize the sending of a file, extending the BaseModel.
 */
class InitSendFile : public BaseModel
{
  public:
    std::string filename;
    size_t fileSize;

    /**
     * @brief Default constructor for InitSendFile.
     * @details Initializes the InitSendFile with the INIT_SEND_FILE message type.
     */
    InitSendFile() : BaseModel(MessageType::INIT_SEND_FILE)
    {
    }

    /**
     * @brief Constructor for InitSendFile with a specified filename and file size.
     * @param filename The name of the file to be sent.
     * @param fileSize The size of the file to be sent.
     */
    InitSendFile(const std::string &filename, size_t fileSize)
        : BaseModel(MessageType::INIT_SEND_FILE), filename(filename), fileSize(fileSize)
    {
    }

    /**
     * @brief Converts the InitSendFile object to a JSON-formatted string.
     * @return A string representing the InitSendFile object in JSON format.
     */
    std::string toJson() const override
    {
        Json::Value root;
        root["filename"] = filename;
        root["fileSize"] = static_cast<Json::UInt64>(fileSize);
        Json::StreamWriterBuilder writer;
        std::string output = Json::writeString(writer, root);
        return output;
    }

    /**
     * @brief Parses a JSON-formatted string to populate the InitSendFile object.
     * @param jsonStr The JSON-formatted string to parse.
     * @throw std::runtime_error if parsing fails.
     */
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