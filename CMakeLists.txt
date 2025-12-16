cmake_minimum_required(VERSION 3.20)
project(tiny_httpserver_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(tiny_httpserver
        src/main.cpp
)

target_link_libraries(tiny_httpserver ws2_32)
