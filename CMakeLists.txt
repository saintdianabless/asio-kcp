cmake_minimum_required(VERSION 3.15)

project(server)

add_subdirectory(thirds/fmt EXCLUDE_FROM_ALL)

set (SOURCE src/main.cpp src/server.cpp src/akcp.cpp src/ikcp.c src/connection_manager.cpp src/server.cpp src/helper.cpp)

add_executable(server ${SOURCE})
target_include_directories(server PRIVATE include thirds/asio/asio/include)
target_link_libraries(server PRIVATE fmt::fmt)