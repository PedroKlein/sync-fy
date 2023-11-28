#include "connectionHandler.hpp"

// TODO: Implement user connection structure with thread safe operations
// TODO: Implement other connection callbacks for each socket type
void ConnectionHandler::onCommandSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {
        common::TCPSocket clientSocket(clientSocketId);
        command::MessageHandler handler(clientSocket);

        ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
        UserConnection &userConnection = connectionHandler.addUserConnection(handler.getUsername());
        ClientConnection &clientConnection = userConnection.addClientConnection(ip);

        userConnection.setCommandConnection(clientConnection, clientSocketId);

        clientSocket.setOnDisconnect([&ip, &userConnection, &connectionHandler, &handler]() {
            std::cout << "Command socket disconnected" << std::endl;
            userConnection.removeClientConnection(ip);
            connectionHandler.removeUserConnection(handler.getUsername());
        });

        handler.monitorMessages();
    }).detach();
}

void ConnectionHandler::onClientDataSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {
        common::TCPSocket clientSocket(clientSocketId);
        clientMonitor::MessageHandler handler(clientSocket);

        ConnectionHandler &connectionHandler = ConnectionHandler::getInstance();
        UserConnection &userConnection = connectionHandler.addUserConnection(handler.getUsername());
        ClientConnection &clientConnection = userConnection.addClientConnection(ip);

        userConnection.setClientDataConnection(clientConnection, clientSocketId);

        clientSocket.setOnDisconnect([&ip, &userConnection, &connectionHandler, &handler]() {
            std::cout << "Client data socket disconnected" << std::endl;
            userConnection.removeClientConnection(ip);
            connectionHandler.removeUserConnection(handler.getUsername());
        });

        handler.monitorMessages();
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
    auto it = userConnections.find(username);
    if (it == userConnections.end())
    {
        throw std::out_of_range("Username not found");
    }
    return *(it->second);
}

void ConnectionHandler::removeUserConnection(const std::string &username)
{
    std::lock_guard<std::mutex> lock(mtx);
    size_t numErased = userConnections.erase(username);
}
