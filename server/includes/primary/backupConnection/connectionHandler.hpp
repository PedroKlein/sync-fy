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

using FileChangesQueue = common::AtomicQueue<common::FileChange>;

class ConnectionHandler
{
  public:
    void ConnectionHandler::onNetworkBackupSocketConnection(int clientSocketId, const std::string &ip)
    {
        std::thread([clientSocketId, ip]() {
            common::TCPSocket clientSocket(clientSocketId);
            // command::MessageHandler handler(clientSocket, ip);

            // ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
            // UserConnection &userConnection = connectionHandler.addUserConnection(handler.getUsername());
            // ClientConnection &clientConnection = userConnection.addClientConnection(ip);

            // userConnection.setCommandConnection(clientConnection, clientSocketId);

            // clientSocket.setOnDisconnect([&ip, &userConnection, &connectionHandler, &handler]() {
            //     std::cout << "Command socket disconnected - " << handler.getUsername() << ":" << ip << std::endl;
            //     handler.stopMonitoring();
            //     connectionHandler.removeUserConnection(handler.getUsername(), ip);
            // });

            // handler.monitorMessages();
        }).detach();
    }

    void ConnectionHandler::onBackupDataSocketConnection(int clientSocketId, const std::string &ip)
    {
    }

    ConnectionHandler &ConnectionHandler::getInstance()
    {
        static ConnectionHandler instance;
        return instance;
    }

    UserConnection &ConnectionHandler::addUserConnection(const std::string &username)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = userConnections.find(username);

        // user connection already exists
        if (it != userConnections.end())
        {
            return *(it->second);
        }

        std::unique_ptr<UserConnection> userConnection = std::make_unique<UserConnection>();

        // Get a reference before moving the unique_ptr into the map
        UserConnection &connectionRef = *userConnection;

        userConnections[username] = std::move(userConnection);

        return connectionRef;
    }

    UserConnection &ConnectionHandler::getUserConnection(const std::string &username)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = userConnections.find(username);
        if (it == userConnections.end())
        {
            throw std::out_of_range("Username not found");
        }
        return *(it->second);
    }

    void ConnectionHandler::removeUserConnection(const std::string &username, const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = userConnections.find(username);

        if (it == userConnections.end())
        {
            return;
        }

        it->second->removeClientConnection(ip);

        if (!it->second->hasClientConnections())
        {
            userConnections.erase(username);
        }
    }

  private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    ConnectionHandler() = default;

    // username -> userConnection
    std::unordered_map<std::string, std::unique_ptr<UserConnection>> userConnections;
    std::mutex mtx;
};