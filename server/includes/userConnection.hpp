#include "fileChangesQueue.hpp"
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

struct Connection
{
    int socketId;
};

struct ClientConnection
{
    std::unique_ptr<Connection> commandConnection;
    std::unique_ptr<Connection> serverDataConnection;
    std::unique_ptr<Connection> clientDataConnection;
};

class UserConnection
{
  public:
    UserConnection() = default;
    ~UserConnection() = default;

    ClientConnection &addClientConnection(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);

        auto it = clientConnections.find(ip);
        // connection already exists
        if (it != clientConnections.end())
        {
            return *(it->second);
        }

        if (clientConnections.size() >= 2)
        {
            throw std::out_of_range("Maximum number of connections reached");
        }

        std::unique_ptr<ClientConnection> clientConnection = std::make_unique<ClientConnection>();

        // Get a reference before moving the unique_ptr into the map
        ClientConnection &connectionRef = *clientConnection;

        clientConnections[ip] = std::move(clientConnection);

        return connectionRef;
    }

    void removeClientConnection(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        clientConnections.erase(ip);
    }

    void setCommandConnection(ClientConnection &clientConnection, int socketId)
    {
        clientConnection.commandConnection = std::make_unique<Connection>();
        clientConnection.commandConnection->socketId = socketId;
    }

    void setClientDataConnection(ClientConnection &clientConnection, int socketId)
    {
        clientConnection.clientDataConnection = std::make_unique<Connection>();
        clientConnection.clientDataConnection->socketId = socketId;
    }

    void setServerDataConnection(ClientConnection &clientConnection, int socketId)
    {
        clientConnection.serverDataConnection = std::make_unique<Connection>();
        clientConnection.serverDataConnection->socketId = socketId;
    }

    ClientConnection &getClientConnection(const std::string &ip)
    {
        auto it = clientConnections.find(ip);
        if (it == clientConnections.end())
        {
            throw std::out_of_range("IP not found");
        }
        return *(it->second);
    }

  private:
    // ip -> clientConnection
    std::map<std::string, std::unique_ptr<ClientConnection>> clientConnections;
    std::mutex mtx;
    // FileChangesQueue fileChangesQueue;
};