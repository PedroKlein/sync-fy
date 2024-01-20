#pragma once

#include <models/connectedNodes.hpp>
#include <string>
#include <vector>

namespace backup
{
// TODO: make the operations here atomic
struct BackupState
{
    int serverId;
    std::string primaryServerAddress;

    std::vector<std::string> connectedClientIps;
    std::vector<common::Node> connectedBackupNodes;
};
} // namespace backup
