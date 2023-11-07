# Define the target executable
add_executable(client
    main.cpp
)

# Link the client executable with necessary libraries, e.g., Boost
target_link_libraries(client ${Boost_LIBRARIES})
