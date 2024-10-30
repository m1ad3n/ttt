cmake_minimum_required(VERSION 3.30.5 FATAL_ERROR)

project(ttt)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(ttt_gui)
add_executable(ttt_cli)

target_link_libraries(ttt_gui PRIVATE SDL2 SDL2main SDL2_image SDL2_ttf)

add_subdirectory(src)