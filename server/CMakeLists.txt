add_executable(server
    src/command/messageHandler.cpp
    src/clientMonitor/messageHandler.cpp
    src/connectionHandler.cpp
    src/serverSocket.cpp
    src/main.cpp
)

target_include_directories(server
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_link_libraries(server jsoncpp pthread common_lib)
