add_executable(server
    main.cpp
)

add_dependencies( server common_lib)

target_link_libraries(server PRIVATE common_lib)