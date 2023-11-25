#include "connectionHandler.hpp"

// TODO: Implement user connection structure with thread safe operations
// TODO: Implement other connection callbacks for each socket type
void ConnectionHandler::onCommandSocketConnection(int clientSocketId, const std::string &ip)
{
    std::thread([clientSocketId, ip]() {
        common::TCPSocket clientSocket(clientSocketId);
        command::MessageHandler handler(clientSocket);
        handler.monitorMessages();
    }).detach();
}
