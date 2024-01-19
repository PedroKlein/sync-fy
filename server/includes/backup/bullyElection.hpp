#include <iostream>
#include <vector>

class ElectionMessageHandler
{
    // Implementation of TCP socket message handling methods
};

class BullyElection
{
  private:
    ElectionMessageHandler &messageHandler;
    int serverId;
    std::vector<int> connectedServers;

  public:
    BullyElection(ElectionMessageHandler &handler, int id) : messageHandler(handler), serverId(id)
    {
    }

    void addServer(int serverId)
    {
        connectedServers.push_back(serverId);
    }

    void removeServer(int serverId)
    {
        for (auto it = connectedServers.begin(); it != connectedServers.end(); ++it)
        {
            if (*it == serverId)
            {
                connectedServers.erase(it);
                break;
            }
        }
    }

    void performElection()
    {
        // Bully election algorithm implementation
    }

    void sendElectionMessage(int serverId)
    {
        // Send election message to a specific server
        // ...
    }

    void sendCoordinatorMessage(int serverId)
    {
        // Send coordinator message to a specific server
        // ...
    }

    void receiveMessage()
    {
        // Receive and handle incoming messages
        // ...
    }
};
