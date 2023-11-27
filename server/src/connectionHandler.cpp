#include "connectionHandler.hpp"

// TODO: Implement user connection structure with thread safe operations
// TODO: Implement other connection callbacks for each socket type
void ConnectionHandler::onCommandSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {  
        common::TCPSocket clientSocket(clientSocketId);
        command::MessageHandler handler(clientSocket);

        ConnectionHandler *connectionHandler = ConnectionHandler::getInstance();

        UserConnection *userConnection = connectionHandler->addUserConnection(handler.getUsername());

        ClientConnection *clientConnection = userConnection->addClientConnection(ip);

        Connection *connection = new Connection();
        connection->socketId = clientSocketId;
        userConnection->setCommandConnection(clientConnection, connection);

        handler.monitorMessages();
    }).detach();
}

void ConnectionHandler::onClientDataSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {
        common::TCPSocket clientSocket(clientSocketId);
        clientMonitor::MessageHandler handler(clientSocket);

        ConnectionHandler *connectionHandler = ConnectionHandler::getInstance();

        UserConnection *userConnection = connectionHandler->addUserConnection(handler.getUsername());

        ClientConnection *clientConnection = userConnection->addClientConnection(ip);

        Connection *connection = new Connection();
        connection->socketId = clientSocketId;
        userConnection->setClientDataConnection(clientConnection, connection);

        handler.monitorMessages();
    }).detach();
}
