#include "connectionHandler.hpp"

void ConnectionHandler::onCommandSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {
        common::TCPSocket clientSocket(clientSocketId);
        command::MessageHandler handler(clientSocket, ip);

        ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
        UserConnection &userConnection = connectionHandler.addUserConnection(handler.getUsername());
        ClientConnection &clientConnection = userConnection.addClientConnection(ip);

        userConnection.setCommandConnection(clientConnection, clientSocketId);

        clientSocket.setOnDisconnect([&ip, &userConnection, &connectionHandler, &handler]() {
            std::cout << "Command socket disconnected - " << handler.getUsername() << ":" << ip << std::endl;
            handler.stopMonitoring();
            connectionHandler.removeUserConnection(handler.getUsername(), ip);
        });

        handler.monitorMessages();
    }).detach();
}

void ConnectionHandler::onClientDataSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {
        common::TCPSocket clientSocket(clientSocketId);
        clientMonitor::MessageHandler handler(clientSocket, ip);

        ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
        UserConnection &userConnection = connectionHandler.addUserConnection(handler.getUsername());
        ClientConnection &clientConnection = userConnection.addClientConnection(ip);

        userConnection.setClientDataConnection(clientConnection, clientSocketId);

        clientSocket.setOnDisconnect([&ip, &userConnection, &connectionHandler, &handler]() {
            std::cout << "Client data socket disconnected - " << handler.getUsername() << ":" << ip << std::endl;
            handler.stopMonitoring();
            connectionHandler.removeUserConnection(handler.getUsername(), ip);
        });

        handler.monitorMessages();
    }).detach();
}

void ConnectionHandler::onServerDataSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {
        common::TCPSocket clientSocket(clientSocketId);
        ServerMessageHandler handler(clientSocket, ip);

        ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
        UserConnection &userConnection = connectionHandler.addUserConnection(handler.getUsername());
        ClientConnection &clientConnection = userConnection.addClientConnection(ip);

        userConnection.setServerDataConnection(clientConnection, clientSocketId);

        localMonitor::LocalMonitor localMonitor(handler, userConnection.getFileChangesQueue(ip));

        clientSocket.setOnDisconnect([&ip, &userConnection, &connectionHandler, &handler, &localMonitor]() {
            std::cout << "Server data socket disconnected - " << handler.getUsername() << ":" << ip << std::endl;
            localMonitor.stopMonitoring();
            connectionHandler.removeUserConnection(handler.getUsername(), ip);
        });

        localMonitor.monitorChanges();
    }).detach();
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
