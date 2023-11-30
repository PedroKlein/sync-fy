#pragma once

#include <string>

namespace common
{
constexpr size_t COMMAND_PORT = 8765;
constexpr size_t CLIENT_DATA_PORT = 8766;
constexpr size_t SERVER_DATA_PORT = 8767;

const std::string HOME_DIR = getenv("HOME");

// for release version
// const std::string DEFAULT_CLIENT_SYNC_DIR = HOME_DIR + "/sync_dir/";
// const std::string DEFAULT_SERVER_SYNC_DIR = HOME_DIR + "/";

const std::string DEFAULT_CLIENT_SYNC_DIR = "./sync_dir/";
const std::string DEFAULT_SERVER_SYNC_DIR = "./";

} // namespace common
