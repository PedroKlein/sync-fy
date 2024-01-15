#pragma once

#include <string>

namespace common
{
constexpr size_t COMMAND_PORT = 8765;
constexpr size_t CLIENT_DATA_PORT = 8766;
constexpr size_t SERVER_DATA_PORT = 8767;

constexpr size_t CLIENT_RECOVERY_PORT = 8768;

const std::string HOME_DIR = getenv("HOME");

#ifdef RELEASE_BUILD
const std::string DEFAULT_CLIENT_SYNC_DIR = HOME_DIR + "/sync_dir/";
const std::string DEFAULT_SERVER_SYNC_DIR = HOME_DIR + "/";
#else
const std::string DEFAULT_CLIENT_SYNC_DIR = "./sync_dir/";
const std::string DEFAULT_SERVER_SYNC_DIR = "./";
#endif

} // namespace common
