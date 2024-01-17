add_executable(client 
    src/cli/messageHandler.cpp
    src/cli/commandHandler.cpp 
    src/cli/cli.cpp 
    src/localMonitor/fileWatcher.cpp
    src/localMonitor/localMonitor.cpp
    src/serverMonitor/messageHandler.cpp
    src/serverMonitor/serverMonitor.cpp
    src/main.cpp
)


target_include_directories(client
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_link_libraries(client PRIVATE pthread common_lib)
