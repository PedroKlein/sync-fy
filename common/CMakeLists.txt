# Create a library target for the common code
add_library(common_lib
    src/database.cpp
    include/database.h
)

# Set include directories for the library
target_include_directories(common_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(common_lib PUBLIC sqlite_orm::sqlite_orm)
