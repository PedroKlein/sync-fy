project(Common)

# Define a library target for the common code
add_library(common_lib
    include/filesystem/file.hpp
    include/datetime/dateTime.hpp
    include/models/deleteFile.hpp
    include/models/initReceiveFile.hpp
    include/models/initBackupData.hpp
    include/models/initSendFile.hpp
    include/models/listFiles.hpp
    include/models/login.hpp
    include/models/serverId.hpp
    include/socket/tcpSocket.hpp
    include/socket/serverSocket.hpp
    include/socket/clientSocket.hpp
    include/constants.hpp
)

find_package(jsoncpp REQUIRED)
target_link_libraries(common_lib ${jsoncpp_LIBRARIES})
target_include_directories(common_lib PUBLIC ${jsoncpp_INCLUDE_DIRS})

target_include_directories(common_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(common_lib PROPERTIES LINKER_LANGUAGE CXX)

message("FOUND CONFIG: ${jsoncpp_CONFIG}")
message("INCLUDE DIRS: ${jsoncpp_INCLUDE_DIRS}")
message("LIBRARIES: ${jsoncpp_LIBRARIES}")