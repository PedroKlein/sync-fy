#pragma once

#include "command/messageHandler.hpp"
#include "userConnection.hpp"
#include <messages/messageHandler.hpp>
#include <socket/tcpSocket.hpp>
#include <thread>
#include <type_traits>

class ConnectionHandler
{
  public:
    // Delete the copy constructor and assignment operator
    ConnectionHandler(const ConnectionHandler &) = delete;
    ConnectionHandler &operator=(const ConnectionHandler &) = delete;

    // Provide a static method to get the instance of the class
    static ConnectionHandler &getInstance()
    {
        static ConnectionHandler instance;
        return instance;
    }

    void setUserConnection(const std::string &username, const UserConnection &userConnection)
    {
        userConnections[username] = userConnection;
    }

    UserConnection getUserConnection(const std::string &username) const
    {
        auto it = userConnections.find(username);
        if (it == userConnections.end())
        {
            throw std::out_of_range("Username not found");
        }
        return it->second;
    }

    void onCommandSocketConnection(int clientSocketId);

  private:
    // Make the constructor private
    ConnectionHandler() = default;

    std::unordered_map<std::string, UserConnection> userConnections;
};