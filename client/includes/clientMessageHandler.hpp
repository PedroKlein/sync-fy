#pragma once

#include <constants.hpp>
#include <filesystem/file.hpp>
#include <messages/fileMessageHandler.hpp>
#include <messages/message.hpp>
#include <models/deleteFile.hpp>
#include <models/initReceiveFile.hpp>
#include <models/initSendFile.hpp>
#include <models/listFiles.hpp>
#include <models/login.hpp>
#include <socket/tcpSocket.hpp>

/**
 * @class ClientMessageHandler
 * @brief Handles client-specific messages and interactions with the server.
 */
class ClientMessageHandler : public common::FileMessageHandler
{
  public:
    /**
     * @brief Constructs a ClientMessageHandler object.
     * @param socket The TCP socket used for communication.
     * @param username The username associated with the client.
     */
    ClientMessageHandler(common::TCPSocket &socket, std::string username)
        : common::FileMessageHandler(socket, username, common::DEFAULT_CLIENT_SYNC_DIR)
    {
        sendLoginMessage(username);
        onLogin();
    }

    /**
     * @brief Sends a login message to the server.
     * @param username The username for the login message.
     */
    void sendLoginMessage(const std::string &username)
    {
        common::Login login(username);
        sendModelMessage(login);
    }

  protected:
    /**
     * @brief Callback function invoked after a successful login.
     */
    virtual void onLogin(){};
};