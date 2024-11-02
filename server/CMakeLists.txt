add_executable(server
    src/primary/command/messageHandler.cpp
    src/primary/clientMonitor/messageHandler.cpp
    src/primary/localMonitor/localMonitor.cpp
    src/primary/clientConnection/clientConnectionHandler.cpp
    src/main.cpp
)

target_include_directories(server
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_link_libraries(server PRIVATE pthread common_lib)
