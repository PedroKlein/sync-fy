
#pragma once

#include <models/baseModel.hpp>

namespace common
{
/**
 * @class Login
 * @brief Represents a login request, extending the BaseModel.
 */
class Login : public BaseModel
{
  public:
    std::string username;

    /**
     * @brief Default constructor for Login.
     * @details Initializes the Login with the LOGIN message type.
     */
    Login() : BaseModel(MessageType::LOGIN)
    {
    }

    /**
     * @brief Constructor for Login with a specified username.
     * @param username The username for the login request.
     */
    Login(const std::string &username) : BaseModel(MessageType::LOGIN), username(username)
    {
    }

    /**
     * @brief Converts the Login object to a JSON-formatted string.
     * @return A string representing the Login object in JSON format.
     */
    std::string toJson() const override
    {
        Json::Value root;
        root["username"] = username;
        Json::StreamWriterBuilder writer;
        std::string output = Json::writeString(writer, root);
        return output;
    }

     /**
     * @brief Parses a JSON-formatted string to populate the Login object.
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
        username = root["username"].asString();
    }
};
} // namespace common