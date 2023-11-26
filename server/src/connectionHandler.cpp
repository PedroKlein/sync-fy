#include "connectionHandler.hpp"

// TODO: Implement user connection structure with thread safe operations
// TODO: Implement other connection callbacks for each socket type
void ConnectionHandler::onCommandSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {  
        common::TCPSocket clientSocket(clientSocketId);
        command::MessageHandler handler(clientSocket);    

        ConnectionHandler *connectionHandler = ConnectionHandler::getInstance();
        Connection connection;
        connection.socketId = clientSocketId;

        UserConnection *userConnection = connectionHandler->getUserConnection(handler.getUsername());
        if (userConnection == nullptr)
        {
            userConnection = new UserConnection();
        }

        ClientConnection *clientConnection = userConnection->getClientConnection(ip);
        if (clientConnection == nullptr)
        {
            clientConnection = new ClientConnection();
            userConnection->setClientConnection(ip, clientConnection);
        }
        else {
            userConnection->setCommandConnection(ip, &connection);
        }

        connectionHandler->addUserConnection(handler.getUsername(), userConnection);

        handler.monitorMessages();
    }).detach();
}
