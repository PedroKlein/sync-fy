cmake_minimum_required(VERSION 3.23)
project(SyncFy)

set(CMAKE_CXX_STANDARD 17)

find_package(jsoncpp REQUIRED)

if(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE_BUILD)
endif()

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
