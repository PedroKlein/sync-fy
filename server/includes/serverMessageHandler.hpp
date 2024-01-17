#pragma once

#include "models/login.hpp"
#include <filesystem/file.hpp>
#include <messages/fileMessageHandler.hpp>
#include <messages/message.hpp>

/**
 * @class ServerMessageHandler
 * @brief Represents a message handler for the server.
 *
 * This class extends the common::FileMessageHandler and provides functionality
 * for handling messages specific to the server.
 */
class ServerMessageHandler : public common::FileMessageHandler
{
  public:
    /**
     * @brief Constructs a ServerMessageHandler object.
     *
     * Initializes the object with a TCP socket and the IP address of the client.
     * Performs login, creates a synchronization folder, and prints login details.
     *
     * @param socket A reference to the common::TCPSocket object.
     * @param ip The IP address of the client.
     */
    ServerMessageHandler(common::TCPSocket &socket, std::string ip) : common::FileMessageHandler(socket)
    {
        this->username = receiveLoginMessage();
        onLogin();
        this->syncFolder = common::DEFAULT_SERVER_SYNC_DIR + username + "/";

        try
        {
            std::filesystem::create_directory(this->syncFolder);
        }
        catch (const std::filesystem::filesystem_error &ex)
        {
            std::cerr << "Error creating directory: " << ex.what() << std::endl;
        }

        this->ip = ip;

        std::cout << "Logged in as " << username << " from " << ip << std::endl;
    }

  protected:
    std::string ip;

    /**
     * @brief Callback function called after successful login.
     *
     * Subclasses can override this function to perform additional actions
     * after a successful login.
     */
    virtual void onLogin(){};

  private:
    /**
     * @brief Receives and processes the login message from the client.
     * @return The username extracted from the login message.
     * @throws std::runtime_error if the received message is not a login message.
     */
    std::string receiveLoginMessage()
    {
        common::MessageHeader header = receiveHeader();

        if (header.messageType != common::MessageType::LOGIN)
        {
            throw std::runtime_error("Expected login message");
        }

        std::vector<char> messageData(header.dataSize);
        socket.receive(messageData.data(), header.dataSize);
        std::string message(messageData.begin(), messageData.end());

        common::Login login;
        login.fromJson(message);

        return login.username;
    }
};