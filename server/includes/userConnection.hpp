#include "fileChangesQueue.hpp"
#include <map>
#include <mutex>
#include <string>
#include <thread>

struct Connection
{
    int socketId;
};

struct ClientConnection
{
    Connection *commandConnection;
    Connection *serverDataConnection;
    Connection *clientDataConnection;
};

class UserConnection
{
  public:
    UserConnection() = default;
    ~UserConnection() = default;

    ClientConnection *addClientConnection(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);

        auto it = clientConnections.find(ip);
        // connection already exists
        if (it != clientConnections.end())
        {
            return it->second;
        }

        if (clientConnections.size() >= 2)
        {
            throw std::out_of_range("Maximum number of connections reached");
        }

        ClientConnection *clientConnection = new ClientConnection();

        clientConnections[ip] = clientConnection;

        return clientConnection;
    }

    void setCommandConnection(ClientConnection *clientConnection, Connection *commandConnection)
    {
        clientConnection->commandConnection = commandConnection;
    }

    void setClientDataConnection(ClientConnection *clientConnection, Connection *clientDataConnection)
    {
        clientConnection->clientDataConnection = clientDataConnection;
    }

    void setServerDataConnection(ClientConnection *clientConnection, Connection *serverDataConnection)
    {
        clientConnection->serverDataConnection = serverDataConnection;
    }

    ClientConnection *getClientConnection(const std::string &ip)
    {
        auto it = clientConnections.find(ip);
        if (it == clientConnections.end())
        {
            throw std::out_of_range("IP not found");
        }
        return it->second;
    }

  private:
    // ip -> clientConnection
    std::map<std::string, ClientConnection *> clientConnections;
    std::mutex mtx;
    // FileChangesQueue fileChangesQueue;
};