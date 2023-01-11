cmake_minimum_required(VERSION 3.24)
project(project_2 C)

set(CMAKE_C_STANDARD 11)

add_executable(server src/server.c src/server.h)
add_executable(client src/client.c src/client.h)

target_link_libraries(server ws2_32)
target_link_libraries(client ws2_32)