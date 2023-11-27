#pragma once

#include "clientMonitor/messageHandler.hpp"
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
        // should we use a mutex here?
        static ConnectionHandler *instance = new ConnectionHandler();
        return instance;
    }

    UserConnection *addUserConnection(const std::string &username)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = userConnections.find(username);

        // user connection already exists
        if (it != userConnections.end())
        {
            return it->second;
        }

        UserConnection *userConnection = new UserConnection();
        userConnections[username] = userConnection;

        return userConnection;
    }

    UserConnection *getUserConnection(const std::string &username) 
    {
        auto it = userConnections.find(username);
        if (it == userConnections.end())
        {
            throw std::out_of_range("Username not found");
        }
        return it->second;
    }

    static void onCommandSocketConnection(int clientSocketId, const std::string &ip);
    static void onClientDataSocketConnection(int clientSocketId, const std::string &ip);

  private:
    // // Make the constructor private
    ConnectionHandler() = default;

    // username -> userConnection
    std::unordered_map<std::string, UserConnection*> userConnections;
    std::mutex mtx;
};