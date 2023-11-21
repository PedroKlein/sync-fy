add_executable(client 
    src/clientSocket.cpp 
    src/cli/commandHandler.cpp 
    src/cli/cliMessageHandler.cpp
    src/cli/clientCommandHandler.cpp 
    src/cli/cli.cpp 
    src/main.cpp
)


target_include_directories(client
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_link_libraries(client jsoncpp pthread common_lib)
