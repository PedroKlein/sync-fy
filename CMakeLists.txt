cmake_minimum_required(VERSION 3.23)
project(SyncFy)

set(CMAKE_CXX_STANDARD 23)

find_package(jsoncpp REQUIRED)
find_package(SqliteOrm REQUIRED)


add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
