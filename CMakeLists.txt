cmake_minimum_required(VERSION 3.23)
project(SyncFy)

set(CMAKE_CXX_STANDARD 23)

find_package(jsoncpp REQUIRED)

add_subdirectory(server)
add_subdirectory(client)