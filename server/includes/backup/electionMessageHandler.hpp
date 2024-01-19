#include "common/messageHandler.hpp"

namespace backup
{
#include <functional>

using Callback = std::function<void()>;

class ElectionMessageHandler : public common::MessageHandler
{
  public:
    void setAliveCallback(const Callback &callback)
    {
        aliveCallback = callback;
    }

    void setCoordinatorCallback(const Callback &callback)
    {
        coordinatorCallback = callback;
    }

    void setElectionCallback(const Callback &callback)
    {
        electionCallback = callback;
    }

    void sendAliveMessage()
    {
        common::Message message(common::MessageType::ALIVE);
        sendMessage(message);
    }

    void sendCoordinatorMessage()
    {
        common::Message message(common::MessageType::COORDINATOR);
        sendMessage(message);
    }

    void sendElectionMessage()
    {
        common::Message message(common::MessageType::ELECTION);
        sendMessage(message);
    }

    void handlePureHeaderMessage(const MessageHeader &header) const
    {
        switch (header.messageType)
        {
        case common::MessageType::ALIVE:
            if (aliveCallback)
                aliveCallback();
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

  private:
    Callback aliveCallback;
    Callback coordinatorCallback;
    Callback electionCallback;
};

} // namespace backup
