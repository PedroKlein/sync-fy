#pragma once

#include <atomic/atomicQueue.hpp>
#include <filesystem/fileChange.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

using FileChangesQueue = common::AtomicQueue<common::FileChange>;

struct ClientConnection
{
    std::shared_ptr<FileChangesQueue> fileChangesQueue;
};

/**
 * @class UserConnection
 * @brief Class managing user connections and associated client connections.
 */
class UserConnection
{
  public:
    /**
     * @brief Default constructor for UserConnection.
     */
    UserConnection() = default;

    /**
     * @brief Default destructor for UserConnection.
     */
    ~UserConnection() = default;

    /**
     * @brief Adds a client connection for a given IP address.
     * @param ip The IP address of the client.
     * @return Reference to the added or existing client connection.
     * @throws std::out_of_range if the maximum number of connections is reached.
     */
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

        clientConnection->fileChangesQueue = std::make_shared<FileChangesQueue>();

        // Get a reference before moving the unique_ptr into the map
        ClientConnection &connectionRef = *clientConnection;

        clientConnections[ip] = std::move(clientConnection);

        return connectionRef;
    }

    /**
     * @brief Removes a client connection for a given IP address.
     * @param ip The IP address of the client to be removed.
     */
    void removeClientConnection(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        clientConnections.erase(ip);
    }

    /**
     * @brief Checks if there are any client connections.
     * @return True if there are client connections, false otherwise.
     */
    bool hasClientConnections()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return clientConnections.size() > 0;
    }

    /**
     * @brief Gets the client connection for a given IP address.
     * @param ip The IP address of the client.
     * @return Reference to the client connection.
     * @throws std::out_of_range if the IP address is not found.
     */
    ClientConnection &getClientConnection(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = clientConnections.find(ip);
        if (it == clientConnections.end())
        {
            throw std::out_of_range("IP not found");
        }
        return *(it->second);
    }

    /**
     * @brief Adds a file change to the file changes queues of other clients (if any).
     * @param ip The IP address of the client originating the file change.
     * @param fileChange The file change to be added.
     * @throws std::out_of_range if the IP address is not found.
     */
    void addFileChange(const std::string &ip, common::FileChange fileChange)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = clientConnections.find(ip);
        if (it == clientConnections.end())
        {
            throw std::out_of_range("IP not found");
        }
        for (auto &clientConnection : clientConnections)
        {
            if (clientConnection.first != ip)
            {
                clientConnection.second->fileChangesQueue->push(fileChange);
            }
        }
    }

    /**
     * @brief Adds a file change to the file changes queues of all clients.
     * @param fileChange The file change to be added.
     */
    void addFileChange(common::FileChange fileChange)
    {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto &clientConnection : clientConnections)
        {
            clientConnection.second->fileChangesQueue->push(fileChange);
        }
    }

    /**
     * @brief Gets the file changes queue for a given client.
     * @param ip The IP address of the client.
     * @return Shared pointer to the file changes queue.
     * @throws std::out_of_range if the IP address is not found.
     */
    std::shared_ptr<FileChangesQueue> getFileChangesQueue(const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = clientConnections.find(ip);
        if (it == clientConnections.end())
        {
            throw std::out_of_range("IP not found");
        }
        return (it->second->fileChangesQueue);
    }

  private:
    // ip -> clientConnection
    std::map<std::string, std::unique_ptr<ClientConnection>> clientConnections;
    std::mutex mtx;
};