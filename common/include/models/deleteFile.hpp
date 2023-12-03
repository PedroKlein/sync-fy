#pragma once

namespace common
{
/**
 * @class DeleteFile
 * @brief Represents a request to delete a file, extending the BaseModel.
 */
class DeleteFile : public BaseModel
{
  public:
    std::string filename;

    /**
     * @brief Default constructor for DeleteFile.
     * @details Initializes the DeleteFile with the DELETE_FILE message type.
     */
    DeleteFile() : BaseModel(MessageType::DELETE_FILE) {}

    /**
     * @brief Constructor for DeleteFile with a specified filename.
     * @param filename The name of the file to be deleted.
     */
    DeleteFile(const std::string &filename) : BaseModel(MessageType::DELETE_FILE), filename(filename) {}

    /**
     * @brief Converts the DeleteFile object to a JSON-formatted string.
     * @return A string representing the DeleteFile object in JSON format.
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
     * @brief Parses a JSON-formatted string to populate the DeleteFile object.
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