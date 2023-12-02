#pragma once

namespace common
{
/**
 * @class InitReceiveFile
 * @brief Represents a request to initialize the reception of a file, extending the BaseModel.
 */
class InitReceiveFile : public BaseModel
{
  public:
    std::string filename;

    /**
     * @brief Default constructor for InitReceiveFile.
     * @details Initializes the InitReceiveFile with the INIT_RECEIVE_FILE message type.
     */
    InitReceiveFile() : BaseModel(MessageType::INIT_RECEIVE_FILE)
    {
    }

    /**
     * @brief Constructor for InitReceiveFile with a specified filename.
     * @param filename The name of the file to be received.
     */
    InitReceiveFile(const std::string &filename) : BaseModel(MessageType::INIT_RECEIVE_FILE), filename(filename)
    {
    }

    /**
     * @brief Converts the InitReceiveFile object to a JSON-formatted string.
     * @return A string representing the InitReceiveFile object in JSON format.
     */
    std::string toJson() const override
    {
        Json::Value root;
        root["filename"] = filename;
        Json::StreamWriterBuilder writer;
        std::string output = Json::writeString(writer, root);
        return output;
    }

    /**
     * @brief Parses a JSON-formatted string to populate the InitReceiveFile object.
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
    }
};
} // namespace common