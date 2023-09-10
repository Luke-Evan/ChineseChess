cmake_minimum_required(VERSION 3.24)
project(project_2 C)

set(CMAKE_C_STANDARD 11)

add_executable(server src/server.c)
add_executable(chess_red src/chess_red.c src/chess.h)
add_executable(chess_black src/chess_black.c src/chess.h)

target_link_libraries(server ws2_32)
target_link_libraries(chess_red ws2_32)
target_link_libraries(chess_black ws2_32)

#配red的sdl
target_link_libraries(chess_red mingw32)
target_link_libraries(chess_red SDL2)
target_link_libraries(chess_red SDL2main)
target_link_libraries(chess_red SDL2_image)
target_link_libraries(chess_red SDL2_ttf)
target_link_libraries(chess_red SDL2_mixer)
#配black的sdl
target_link_libraries(chess_black mingw32)
target_link_libraries(chess_black SDL2)
target_link_libraries(chess_black SDL2main)
target_link_libraries(chess_black SDL2_image)
target_link_libraries(chess_black SDL2_ttf)
target_link_libraries(chess_black SDL2_mixer)

