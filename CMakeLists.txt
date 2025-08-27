cmake_minimum_required(VERSION 3.31.0)

project(HttpServerCpp
    VERSION 1.0.0
    DESCRIPTION "A HTTP Server written in C++"
    LANGUAGES CXX   
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

set(HTTP_SOURCES
    src/http/HttpRequest.cpp
    src/http/HttpResponse.cpp
    src/http/HttpParser.cpp
    src/http/HttpFormatter.cpp    
)

set(SERVER_SOURCES
    src/server/Router.cpp
    src/server/Server.cpp    
)

set(ALL_SOURCES
    ${HTTP_SOURCES}
    ${SERVER_SOURCES}
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# add_executable(httpservercpp Server.cpp httpDecoder.cpp router.cpp)

# target_link_libraries(httpservercpp ws2_32)
# target_link_libraries(httpclientcpp ws2_32)

target_link_libraries(${PROJECT_NAME} ws2_32)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/pages ${CMAKE_SOURCE_DIR}/pages
    COMMENT "Ensuring pages directory is accessible"
)