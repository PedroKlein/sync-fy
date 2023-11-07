cmake_minimum_required(VERSION 3.10)
project(SyncFy)

set(CMAKE_CXX_STANDARD 17)

set(BOOST_INCLUDE_LIBRARIES thread filesystem system program_options asio)
set(BOOST_ENABLE_CMAKE ON)

# Download and extract the boost library from GitHub
message(STATUS "Downloading and extracting boost library sources. This will take some time...")
include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.83.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Boost)

# FetchContent_Declare(
#     sqlite_orm
#     GIT_REPOSITORY https://github.com/fnc12/sqlite_orm.git
#     GIT_TAG master
# )
# FetchContent_MakeAvailable(sqlite_orm)

# Add subdirectories for the server and client
add_subdirectory(server)
add_subdirectory(client)

# Add subdirectory for tests, if applicable
# add_subdirectory(tests)

# Optionally, configure installation targets
# install(TARGETS server DESTINATION bin)
# install(TARGETS client DESTINATION bin)
