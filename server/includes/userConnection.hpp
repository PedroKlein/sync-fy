#include "fileChangesQueue.hpp"
#include <map>
#include <string>
#include <thread>

struct Connection
{
    common::TCPSocket socket;
    std::thread thread;
};

struct ClientConnection
{
    Connection *commandConnection;
    Connection *serverDataConnection;
    Connection *clientDataConnection;
    FileChangesQueue fileChangesQueue;
};

class UserConnection
{
  private:
    std::map<std::string, ClientConnection *> clientConnections;

  public:
    UserConnection() = default;
    ~UserConnection() = default;

    void setClientConnection(const std::string &ip, const ClientConnection *clientConnection)
    {
        if (clientConnections.size() >= 2)
        {
            throw std::out_of_range("Maximum number of connections reached");
        }

        clientConnections[ip] = clientConnection;
    }

    ClientConnection &getClientConnection(const std::string &ip) const
    {
        auto it = clientConnections.find(ip);
        if (it == clientConnections.end())
        {
            throw std::out_of_range("IP not found");
        }
        return it->second;
    }
};