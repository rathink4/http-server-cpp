cmake_minimum_required(VERSION 3.31.0)

project(httpservercpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(httpservercpp Server.cpp)
add_executable(httpclientcpp Client.cpp)

target_link_libraries(httpservercpp ws2_32)
target_link_libraries(httpclientcpp ws2_32)