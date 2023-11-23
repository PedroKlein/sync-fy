#include "connectionHandler.hpp"

// TODO: Implement user connection structure with thread safe operations
// TODO: Implement other connection callbacks for each socket type
void ConnectionHandler::onCommandSocketConnection(int clientSocketId)
{
    std::thread([clientSocketId]() {
        common::TCPSocket clientSocket(clientSocketId);
        command::MessageHandler handler(clientSocket);
        handler.monitorMessages();
    }).detach();
}
