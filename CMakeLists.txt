cmake_minimum_required(VERSION 3.26)
project(tmp C)

set(CMAKE_C_STANDARD 11)

include_directories(/usr/local/include)
add_executable(
        zap-2023-8748 main.c
        ./karel-the-robot-master/src/karel.c
        ./karel-the-robot-master/src/superkarel.c
        ./karel-the-robot-master/src/internals.c)
target_link_libraries(zap-2023-8748 curses)
