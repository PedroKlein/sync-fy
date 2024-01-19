#pragma once

#include <atomic/atomicQueue.hpp>
#include <filesystem/fileChange.hpp>
#include <memory>

using UserFileChange = std::pair<std::string, common::FileChange>;
using UserFileChangesQueue = common::AtomicQueue<UserFileChange>;

struct BackupConnection
{
    std::string ip;
    int id;
    int connections = 0;
    std::shared_ptr<UserFileChangesQueue> fileChangesQueue;

    BackupConnection(const std::string &ip, int serverId, std::shared_ptr<UserFileChangesQueue> queue)
        : ip(ip), id(getNextId(serverId)), fileChangesQueue(queue)
    {
    }

    static int getNextId(int serverId)
    {
        static int nextId = 0;
        return serverId + (nextId++);
    }
};