cmake_minimum_required(VERSION 3.31.0)

project(httpservercpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(httpservercpp Server.cpp httpDecoder.cpp router.cpp)

target_link_libraries(httpservercpp ws2_32)
# target_link_libraries(httpclientcpp ws2_32)

set_target_properties(httpservercpp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_custom_command(TARGET httpservercpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/pages ${CMAKE_SOURCE_DIR}/pages
    COMMENT "Ensuring pages directory is accessible"
)