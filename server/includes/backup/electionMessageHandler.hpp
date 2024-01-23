#pragma once

#include <messages/messageHandler.hpp>

namespace backup
{
#include <functional>

using Callback = std::function<void()>;

class ElectionMessageHandler : public common::MessageHandler
{
  public:
    ElectionMessageHandler(common::TCPSocket &socket) : common::MessageHandler(socket)
    {
    }

    void setCoordinatorCallback(const Callback &callback)
    {
        coordinatorCallback = callback;
    }

    void setElectionCallback(const Callback &callback)
    {
        electionCallback = callback;
    }

    void sendAliveMessage() const
    {
        common::Message message(common::MessageType::ALIVE);
        sendMessage(message);
    }

    void sendCoordinatorMessage() const
    {
        common::Message message(common::MessageType::COORDINATOR);
        sendMessage(message);
    }

    void sendElectionMessage() const
    {
        common::Message message(common::MessageType::ELECTION);
        sendMessage(message);
    }

    void receiveAliveMessage()
    {
        common::MessageHeader header = receiveHeader();

        if (header.messageType != common::MessageType::ALIVE)
        {
            throw std::runtime_error("Received wrong message type");
        }
    }

  private:
    Callback coordinatorCallback;
    Callback electionCallback;

    void handleMessage(const common::Message &message) override
    {
    }

    void handlePureHeaderMessage(const common::MessageHeader &header) const override
    {
        switch (header.messageType)
        {
        case common::MessageType::ALIVE:
            break;
        case common::MessageType::COORDINATOR:
            if (coordinatorCallback)
                coordinatorCallback();
            break;
        case common::MessageType::ELECTION:
            if (electionCallback)
                electionCallback();
            break;
        default:
            std::cout << "Received unknown message" << std::endl;
            break;
        }
    }
};

} // namespace backup
