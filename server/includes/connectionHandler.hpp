#pragma once

#include "command/messageHandler.hpp"
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
            common::TCPSocket clientSocket(clientSocketId);
            command::MessageHandler handler(clientSocket);
            handler.receiveMessage();
        }).detach();
    }
};