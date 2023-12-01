#pragma once

#include "filesystem/directory.hpp"
#include <vector>

namespace common
{
class ListFiles : public BaseModel
{
  public:
    ListFiles() : BaseModel(MessageType::LIST_FILES)
    {
    }

    std::vector<FileInfo> files;

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