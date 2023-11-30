#pragma once

#include "clientMonitor/messageHandler.hpp"
#include "command/messageHandler.hpp"
#include "localMonitor/localMonitor.hpp"
#include "userConnection.hpp"
#include <memory>
#include <messages/messageHandler.hpp>
#include <socket/tcpSocket.hpp>
#include <thread>
#include <type_traits>

class ConnectionHandler
{
  public:
    UserConnection &addUserConnection(const std::string &username);
    UserConnection &getUserConnection(const std::string &username);

    void removeUserConnection(const std::string &username, const std::string &ip);

    static ConnectionHandler &getInstance();
    static void onCommandSocketConnection(int clientSocketId, const std::string &ip);
    static void onClientDataSocketConnection(int clientSocketId, const std::string &ip);
    static void onServerDataSocketConnection(int clientSocketId, const std::string &ip);

  private:
    // Make the constructor private
    ConnectionHandler() = default;

    // username -> userConnection
    std::unordered_map<std::string, std::unique_ptr<UserConnection>> userConnections;
    std::mutex mtx;
};