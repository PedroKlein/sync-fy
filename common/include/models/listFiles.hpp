#pragma once

#include "filesystem/directory.hpp"
#include <vector>

namespace common
{
/**
 * @class ListFiles
 * @brief Represents a message for listing files in a directory.
 *
 * The ListFiles class is a concrete implementation of the BaseModel and
 * represents a message with information about files in a directory.
 */
class ListFiles : public BaseModel
{
  public:
    /**
     * @brief Constructs a ListFiles object with the message type set to LIST_FILES.
     */
    ListFiles() : BaseModel(MessageType::LIST_FILES)
    {
    }

    std::vector<FileInfo> files;

    /**
     * @brief Converts the ListFiles object to a JSON-formatted string.
     *
     * @return A JSON-formatted string representing the ListFiles object.
     */
    std::string toJson() const override
    {
        Json::Value root;
        for (const auto &file : files)
        {
            Json::Value fileJson;
            fileJson["filename"] = file.filename;
            fileJson["mTime"] = static_cast<Json::Int64>(file.modificationTime);
            fileJson["aTime"] = static_cast<Json::Int64>(file.accessTime);
            fileJson["cTime"] = static_cast<Json::Int64>(file.creationTime);
            fileJson["filesize"] = static_cast<Json::Int64>(file.filesize);
            root.append(fileJson);
        }
        Json::StreamWriterBuilder writer;
        std::string output = Json::writeString(writer, root);
        return output;
    }

    /**
     * @brief Parses a JSON-formatted string to populate the ListFiles object.
     *
     * @param jsonStr The JSON-formatted string to be parsed.
     * @throws std::runtime_error if parsing the JSON fails.
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
        for (const auto &fileJson : root)
        {
            FileInfo file;
            file.filename = fileJson["filename"].asString();
            file.modificationTime = fileJson["mTime"].asUInt64();
            file.accessTime = fileJson["aTime"].asUInt64();
            file.creationTime = fileJson["cTime"].asUInt64();
            file.filesize = fileJson["filesize"].asUInt64();
            files.push_back(file);
        }
    }
};
} // namespace common