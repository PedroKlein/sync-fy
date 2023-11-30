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
            fileJson["mTime"] = file.modificationTime;
            fileJson["aTime"] = file.accessTime;
            fileJson["cTime"] = file.creationTime;
            fileJson["filesize"] = file.filesize;
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