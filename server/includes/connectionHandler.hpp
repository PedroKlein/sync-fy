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
    // // Delete the copy constructor and assignment operator
    // ConnectionHandler(const ConnectionHandler &) = delete;
    // ConnectionHandler &operator=(const ConnectionHandler &) = delete;

    // Provide a static method to get the instance of the class
    static ConnectionHandler *getInstance()
    {
        static ConnectionHandler *instance = new ConnectionHandler();
        return instance;
    }

    void addUserConnection(const std::string &username, UserConnection *userConnection)
    {
        userConnections[username] = userConnection;
    }

    UserConnection *getUserConnection(const std::string &username) 
    {
        auto it = userConnections.find(username);
        if (it == userConnections.end())
        {
            // throw std::out_of_range("Username not found");
            return nullptr;
        }
        return it->second;
    }

    static void onCommandSocketConnection(int clientSocketId, const std::string &ip);

  private:
    // // Make the constructor private
    ConnectionHandler() = default;

    std::unordered_map<std::string, UserConnection*> userConnections;
};