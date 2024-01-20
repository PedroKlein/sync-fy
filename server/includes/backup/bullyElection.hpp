#pragma once

#include "electionMessageHandler.hpp"
#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <models/connectedNodes.hpp>
#include <socket/clientSocket.hpp>
#include <socket/serverSocket.hpp>
#include <socket/tcpSocket.hpp>
#include <vector>

#define ELECTION_SOCKET_PORT 8771
#define TIMEOUT_SECONDS 1
#define SELF_WIN_IP "localhost"

namespace backup
{

using ElectionEndCallback = std::function<void(const std::string &)>;

// This class has high risk of concurrency access problems, it would be good to better separate concerns
class BullyElection
{
  public:
    BullyElection(BackupState &backupState) : backupState(backupState)
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

        for (common::Node node : backupState.connectedBackupNodes)
        {
            if (node.id <= backupState.serverId)
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
        for (common::Node node : backupState.connectedBackupNodes)
        {
            if (node.id == backupState.serverId)
            {
                continue;
            }

            common::ClientSocket electionSocket(node.ip, ELECTION_SOCKET_PORT);
            ElectionMessageHandler electionMessageHandler(electionSocket);

            electionMessageHandler.sendCoordinatorMessage();
        }

        // onElectionEndCallback(SELF_WIN_IP);
    }

    std::thread *listenElections()
    {
        common::ServerSocket electionSocket(ELECTION_SOCKET_PORT);

        electionSocketThread =
            std::thread(&common::ServerSocket::startListening, &electionSocket, [this](int socketId, std::string ip) {
                common::TCPSocket electionSocket(socketId);
                ElectionMessageHandler electionMessageHandler(electionSocket);

                electionMessageHandler.receiveElectionMessage();
                electionMessageHandler.sendAliveMessage();
            });

        return &electionSocketThread;
    }

  private:
    std::thread electionSocketThread;
    BackupState &backupState;
    std::atomic<bool> isElecting{false};

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
                // TODO: hnadle the permessive error here
                //  handler.stopMonitoring();
                // onElectionEndCallback(ip);
            });

            handler.monitorMessages();
        }).detach();
    }
};

} // namespace backup
