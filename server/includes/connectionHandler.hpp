#pragma once

#include "commandMessageHandler.hpp"
#include <messages/messageHandler.hpp>
#include <socket/tcpSocket.hpp>
#include <thread>
#include <type_traits>

class ConnectionHandler
{
  public:
    static void onCommandSocketConnection(int clientSocketId)
    {
        std::thread([clientSocketId]() {
            TCPSocket clientSocket(clientSocketId);
            CommandMessageHandler handler(clientSocket);
            handler.receiveMessage();
        }).detach();
    }
};