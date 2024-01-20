#include "primary/clientConnection/clientConnectionHandler.hpp"

void ClientConnectionHandler::onCommandSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {
        common::TCPSocket clientSocket(clientSocketId);
        command::MessageHandler handler(clientSocket, ip);

        ClientConnectionHandler &connectionHandler = ClientConnectionHandler::getInstance();
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

void ClientConnectionHandler::onClientDataSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {
        common::TCPSocket clientSocket(clientSocketId);
        clientMonitor::MessageHandler handler(clientSocket, ip);

        ClientConnectionHandler &connectionHandler = ClientConnectionHandler::getInstance();
        UserConnection &userConnection = connectionHandler.addUserConnection(handler.getUsername());
        ClientConnection &clientConnection = userConnection.addClientConnection(ip);

        userConnection.setClientDataConnection(clientConnection, clientSocketId);

        // No specific reason for this socket to have this, but one of the three should have it
        BackupConnectionHandler &backupConnectionHandler = BackupConnectionHandler::getInstance();
        BackupState &backupState = BackupState::getInstance();

        backupState.addConectedClientIp(ip);

        clientSocket.setOnDisconnect(
            [&ip, &userConnection, &connectionHandler, &handler, &backupConnectionHandler, &backupState]() {
                std::cout << "Client data socket disconnected - " << handler.getUsername() << ":" << ip << std::endl;
                handler.stopMonitoring();
                connectionHandler.removeUserConnection(handler.getUsername(), ip);

                backupState.removeConnectedClientIp(ip);
            });

        handler.monitorMessages();
    }).detach();
}

void ClientConnectionHandler::onServerDataSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {
        common::TCPSocket clientSocket(clientSocketId);
        ServerMessageHandler handler(clientSocket, ip);

        ClientConnectionHandler &connectionHandler = ClientConnectionHandler::getInstance();
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

ClientConnectionHandler &ClientConnectionHandler::getInstance()
{
    static ClientConnectionHandler instance;
    return instance;
}

UserConnection &ClientConnectionHandler::addUserConnection(const std::string &username)
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

UserConnection &ClientConnectionHandler::getUserConnection(const std::string &username)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = userConnections.find(username);
    if (it == userConnections.end())
    {
        throw std::out_of_range("Username not found");
    }
    return *(it->second);
}

void ClientConnectionHandler::removeUserConnection(const std::string &username, const std::string &ip)
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
