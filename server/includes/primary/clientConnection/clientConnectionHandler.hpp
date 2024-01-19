#pragma once

#include "primary/clientMonitor/messageHandler.hpp"
#include "primary/command/messageHandler.hpp"
#include "primary/localMonitor/localMonitor.hpp"
#include "userConnection.hpp"
#include <memory>
#include <messages/messageHandler.hpp>
#include <socket/tcpSocket.hpp>
#include <thread>
#include <type_traits>

/**
 * @class ClientConnectionHandler
 * @brief Singleton class managing user connections and socket connections.
 */
class ClientConnectionHandler
{
  public:
    /**
     * @brief Adds a user connection for the specified username.
     * @param username The username of the user.
     * @return Reference to the added or existing UserConnection.
     */
    UserConnection &addUserConnection(const std::string &username);

    /**
     * @brief Gets the user connection for the specified username.
     * @param username The username of the user.
     * @return Reference to the UserConnection.
     * @throws std::out_of_range if the user connection does not exist.
     */
    UserConnection &getUserConnection(const std::string &username);

    /**
     * @brief Removes the user connection for the specified username and IP address.
     * @param username The username of the user.
     * @param ip The IP address of the user.
     */
    void removeUserConnection(const std::string &username, const std::string &ip);

    /**
     * @brief Gets the instance of the ClientConnectionHandler singleton.
     * @return Reference to the ClientConnectionHandler instance.
     */
    static ClientConnectionHandler &getInstance();

    /**
     * @brief Handles a connection on the command socket.
     * @param clientSocketId The socket identifier of the connected client.
     * @param ip The IP address of the connected client.
     */
    static void onCommandSocketConnection(int clientSocketId, const std::string &ip);

    /**
     * @brief Handles a connection on the client data socket.
     * @param clientSocketId The socket identifier of the connected client.
     * @param ip The IP address of the connected client.
     */
    static void onClientDataSocketConnection(int clientSocketId, const std::string &ip);

    /**
     * @brief Handles a connection on the server data socket.
     * @param clientSocketId The socket identifier of the connected client.
     * @param ip The IP address of the connected client.
     */
    static void onServerDataSocketConnection(int clientSocketId, const std::string &ip);

  private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    ClientConnectionHandler() = default;

    // username -> userConnection
    std::unordered_map<std::string, std::unique_ptr<UserConnection>> userConnections;
    std::mutex mtx;
};