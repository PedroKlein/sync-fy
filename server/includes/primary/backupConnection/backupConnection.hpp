#pragma once

#include <atomic/atomicQueue.hpp>
#include <filesystem/fileChange.hpp>
#include <memory>

using UserFileChange = std::pair<std::string, common::FileChange>;
using UserFileChangesQueue = common::AtomicQueue<UserFileChange>;

using HasClientAndNodeIpsChange = std::pair<bool, bool>;

struct BackupConnection
{
    std::string ip;
    int id;
    int connections = 0;
    std::shared_ptr<UserFileChangesQueue> fileChangesQueue;
    std::shared_ptr<HasClientAndNodeIpsChange> clientAndNodeChanges;

    BackupConnection(const std::string &ip, int primaryServerId, std::shared_ptr<UserFileChangesQueue> queue,
                     std::shared_ptr<HasClientAndNodeIpsChange> clientAndNodeChanges)
        : ip(ip), id(getNextId(primaryServerId)), fileChangesQueue(queue), clientAndNodeChanges(clientAndNodeChanges)
    {
    }

    static int getNextId(int primaryServerId)
    {
        static int nextId = 0;
        return primaryServerId + (++nextId);
    }
};