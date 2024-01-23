#pragma once

#include "backupState.hpp"
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

#define ELECTION_SOCKET_PORT 8770
#define TIMEOUT_SECONDS 1000
#define SELF_WIN_IP "localhost"

namespace backup
{

using ElectionEndCallback = std::function<void(const std::string &)>;

// This class has high risk of concurrency access problems, it would be good to better separate concerns
class BullyElection
{
  public:
    BullyElection()
    {
    }

    void startElection()
    {
        std::cout << "Starting election" << std::endl;
        if (isElecting.load())
        {
            std::cout << "Already electing" << std::endl;
            return;
        }

        isElecting.store(true);

        std::vector<std::future<void>> futures;
        std::atomic<bool> anyThreadEnded(false);
        BackupState &backupState = BackupState::getInstance();

        for (common::Node node : backupState.getConnectedBackupNodes())
        {
            if (node.id <= backupState.getServerId())
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
        BackupState &backupState = BackupState::getInstance();

        for (common::Node node : backupState.getConnectedBackupNodes())
        {
            if (node.id == backupState.getServerId())
            {
                continue;
            }

            common::ClientSocket electionSocket(node.ip, ELECTION_SOCKET_PORT);
            ElectionMessageHandler electionMessageHandler(electionSocket);

            electionMessageHandler.sendCoordinatorMessage();
        }

        onElectionEndCallback(SELF_WIN_IP);
    }

    std::thread *listenElections()
    {
        common::ServerSocket electionSocket(ELECTION_SOCKET_PORT);

        electionSocketThread = std::thread(
            &common::ServerSocket::startListening, &electionSocket,
            [this](int clientSocketId, std::string ip) { this->onElectionSocketConnection(clientSocketId, ip); });

        return &electionSocketThread;
    }

    void setElectionEndCallback(ElectionEndCallback callback)
    {
        onElectionEndCallback = callback;
    }

    void onElectionSocketConnection(int clientSocketId, const std::string &ip)
    {
        std::thread([clientSocketId, ip, this]() {
            common::TCPSocket clientSocket(clientSocketId);
            std::cout << "Election socket connected to " << ip << std::endl;

            ElectionMessageHandler handler(clientSocket);

            clientSocket.setOnDisconnect([&handler]() { handler.stopMonitoring(); });

            handler.setElectionCallback([handler, this]() {
                handler.sendAliveMessage();
                this->startElection();
            });

            handler.setCoordinatorCallback([clientSocket, ip, this]() {
                // TODO: handle the permessive error here
                // clientSocket.closeConnection();
                onElectionEndCallback(ip);
            });

            handler.monitorMessages();
        }).detach();
    }

  private:
    std::thread electionSocketThread;
    std::atomic<bool> isElecting{false};
    ElectionEndCallback onElectionEndCallback;
};

} // namespace backup
