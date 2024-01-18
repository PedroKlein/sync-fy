#pragma once

#include <memory>

using UserFileChanges = std::pair<std::string, common::AtomicQueue<common::FileChange>>;
using UserFileChangesQueue = common::AtomicQueue<UserFileChanges>;

struct BackupConnection
{
    std::string ip;
    int id;
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