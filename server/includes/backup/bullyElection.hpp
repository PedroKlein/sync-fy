#pragma once

#include "electionMessageHandler.hpp"
#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <socket/clientSocket.hpp>
#include <vector>

#define ELECTION_SOCKET_PORT 8771
#define TIMEOUT_SECONDS 1
#define SELF_WIN_IP "localhost"

namespace backup
{

using ElectionEndCallback = std::function<void(const std::string &)>;
struct ServerNode
{
    int id;
    std::string ip;
};

// This class has high risk of concurrency access problems, it would be good to better separate concerns
class BullyElection
{
  public:
    BullyElection(int serverid, ElectionEndCallback onElectionEndCallback)
        : serverId(serverid), onElectionEndCallback(onElectionEndCallback)
    {
    }

    void startElection()
    {
        if (isElecting.load())
        {
            return;
        }

        isElecting.store(true);

        std::vector<std::future<void>> futures;
        std::atomic<bool> anyThreadEnded(false);

        for (ServerNode node : connectedServers)
        {
            if (node.id < serverId)
            {
                continue;
            }

            futures.emplace_back(std::async(std::launch::async, [this, node, &anyThreadEnded]() {
                common::ClientSocket electionSocket(node.ip, ELECTION_SOCKET_PORT);
                ElectionMessageHandler electionMessageHandler(electionSocket);

                electionMessageHandler.sendElectionMessage();
                electionMessageHandler.receiveAliveMessage();

                anyThreadEnded.store(true);
            }));
        }

        auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(TIMEOUT_SECONDS))
        {
            if (anyThreadEnded.load())
            {
                break;
            }
        }

        if (!anyThreadEnded.load())
        {
            std::cout << "No thread ended within the timeout." << std::endl;
            declareVictory();
        }

        // TODO: If someone answer, wait for a coordinator message for a while, if none comes, declare victory
        // Make this with the isElecting flag accessible on the election message monitor, if the flag is on, it has a
        // timeout to receive a coordinator message. If the timout blows up, it declares victory.

        futures.clear();
    }

    void declareVictory()
    {
        for (ServerNode node : connectedServers)
        {
            common::ClientSocket electionSocket(node.ip, ELECTION_SOCKET_PORT);
            ElectionMessageHandler electionMessageHandler(electionSocket);

            electionMessageHandler.sendCoordinatorMessage();
        }

        onElectionEndCallback(SELF_WIN_IP);
    }

    void listenElections()
    {
        common::ServerSocket electionSocket(ELECTION_SOCKET_PORT);

        std::thread electionSocketThread(&common::ServerSocket::startListening, &electionSocket,
                                         [this](int socketId, std::string ip) {
                                             common::ClientSocket electionSocket(socketId);
                                             ElectionMessageHandler electionMessageHandler(electionSocket);

                                             electionMessageHandler.receiveElectionMessage();
                                             electionMessageHandler.sendAliveMessage();

                                             addConnectedServer({socketId, ip});
                                         });

        electionSocketThread.join();
    }

    void addConnectedServer(ServerNode node)
    {
        connectedServers.push_back(node);
    }

    void removeConnectedServer(ServerNode node)
    {
        // remove by the id
        connectedServers.erase(std::remove_if(connectedServers.begin(), connectedServers.end(),
                                              [node](ServerNode &server) { return server.id == node.id; }),
                               connectedServers.end());
    }

  private:
    ElectionEndCallback onElectionEndCallback;
    ElectionMessageHandler &messageHandler;
    int serverId;
    std::vector<ServerNode> connectedServers;
    std ::atomic<bool> isElecting(false);

    void onElectionSocketConnection(int clientSocketId, const std::string &ip)
    {
        std::thread([clientSocketId, ip, this]() {
            common::TCPSocket clientSocket(clientSocketId);
            ElectionMessageHandler handler(clientSocket);

            handler.setElectionCallback([handler, this]() {
                handler.sendAliveMessage();
                this->startElection();
            });

            handler.setCoordinatorCallback([handler, ip, this]() {
                handler.stopMonitoringMessages();
                onElectionEndCallback(ip);
            });

            handler.monitorMessages();
        }).detach();
    }
};

} // namespace backup
