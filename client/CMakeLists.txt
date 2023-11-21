add_executable(client 
    src/clientSocket.cpp 
    src/commandHandler.cpp 
    src/clientCommandHandler.cpp 
    src/cli.cpp 
    src/main.cpp
)


target_include_directories(client
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_link_libraries(client jsoncpp pthread common_lib)
