#include "connectionHandler.hpp"

// TODO: Implement user connection structure with thread safe operations
// TODO: Implement other connection callbacks for each socket type
void ConnectionHandler::onCommandSocketConnection(int clientSocketId, const std::string &ip)
{
    common::TCPSocket clientSocket(clientSocketId);

    std::thread([clientSocket]() {      
        command::MessageHandler handler(clientSocket);    
        handler.monitorMessages();
    }).detach();

    ConnectionHandler *connectionHandler = ConnectionHandler::getInstance();

    Connection connection;
    connection.socket = clientSocket;

    UserConnection *userConnection = connectionHandler->getUserConnection(ip);
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

    connectionHandler->addUserConnection(ip, userConnection);

}
