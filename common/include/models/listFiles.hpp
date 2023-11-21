#include <vector>

struct FileData
{
    std::string filename;
    std::string mTime;
    std::string aTime;
    std::string cTime;
};

class ListFiles : public BaseModel
{
  public:
    std::vector<FileData> files;

    std::string toJson() const override
    {
        Json::Value root;
        for (const auto &file : files)
        {
            Json::Value fileJson;
            fileJson["filename"] = file.filename;
            fileJson["mTime"] = file.mTime;
            fileJson["aTime"] = file.aTime;
            fileJson["cTime"] = file.cTime;
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
            FileData file;
            file.filename = fileJson["filename"].asString();
            file.mTime = fileJson["mTime"].asString();
            file.aTime = fileJson["aTime"].asString();
            file.cTime = fileJson["cTime"].asString();
            files.push_back(file);
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const ListFiles &obj)
    {
        os << "ListFiles: {\n";
        for (const auto &file : obj.files)
        {
            os << "\tfilename: " << file.filename << ",\n"
               << "\tmTime: " << file.mTime << ",\n"
               << "\taTime: " << file.aTime << ",\n"
               << "\tcTime: " << file.cTime << "\n";
        }
        os << "}";
        return os;
    }
};