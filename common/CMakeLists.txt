project(Common)

# Define a library target for the common code
add_library(common_lib
    include/database/Database.h
    include/datetime/DateTime.h
    include/models/deleteFile.hpp
    include/models/initSendFile.hpp
    include/models/listFiles.hpp
    include/models/login.hpp
    include/socket/tcpSocket.hpp
    include/constants.hpp
)

# Add include directories for the common library
target_include_directories(common_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(common_lib PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(common_lib PUBLIC SQLite::SQLite3)
