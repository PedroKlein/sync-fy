#include "fileChangesQueue.hpp"
#include <map>
#include <string>
#include <thread>

struct Connection
{
    common::TCPSocket socket;
};

struct ClientConnection
{
    Connection *commandConnection;
    Connection *serverDataConnection;
    Connection *clientDataConnection;
    // FileChangesQueue fileChangesQueue;
};

class UserConnection
{
  private:
    std::map<std::string, ClientConnection *> clientConnections;

  public:
    UserConnection() = default;
    ~UserConnection() = default;

    void setClientConnection(const std::string &ip, ClientConnection *clientConnection)
    {
        if (clientConnections.size() >= 2)
        {
            throw std::out_of_range("Maximum number of connections reached");
        }

        clientConnections[ip] = clientConnection;
    }

    void setCommandConnection(const std::string &ip, Connection *commandConnection)
    {
        auto it = clientConnections.find(ip);
        if (it == clientConnections.end())
        {
            throw std::out_of_range("IP not found");
        }

        it->second->commandConnection = commandConnection;
    }

    ClientConnection *getClientConnection(const std::string &ip)
    {
        auto it = clientConnections.find(ip);
        if (it == clientConnections.end())
        {
            // throw std::out_of_range("IP not found");
            return nullptr;
        }
        return it->second;
    }
};