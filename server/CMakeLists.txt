# Define the target executable
add_executable(server
    main.cpp
)

# Link the server executable with necessary libraries, e.g., Boost
target_link_libraries(server PRIVATE Boost::asio)
