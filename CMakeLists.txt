cmake_minimum_required(VERSION 3.26)
project(zap-2023-8748 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_C_FLAGS "-Werror -Wall")
set(CMAKE_C_FLAGS "-Werror=vla -march=native")

set(currentProblemSet 7)
add_compile_definitions(DEBUG_FLAG)

add_executable(ps1 ./ps1/task_1.c) #ps1

add_executable(ps2 ./ps2/arrays.c) #ps2

add_executable(ps3 ./ps3/main.c ./ps3/hangman.c ./ps3/morse.c) #ps3

add_executable(ps4 ./ps4/main.c ./ps4/ballsortpuzzle.c ./ps4/c4.c ./ps4/graphic.c) #ps4
target_link_libraries(ps4 curses)
target_compile_definitions(ps4 PRIVATE
    CURSES_ALLOWED=1
    CUSTOM_ARENA_ALLOWED=1
    CURSES_ALLOWED=1
    HAPPY_GAME_END=1
    CUSTOM_HEADER_ALLOWED=1)

add_executable(ps5 ./ps5/qr.c) #ps5

add_executable(ps6 ps6/main.c ps6/life/engine.c ps6/life/graphic.c ps6/utils/utils.c) #ps6
target_link_libraries(ps6 curses)

add_executable(ps7-1 ps7/count_words.c) #ps7
add_executable(ps7-2 ps7/every_second.c) #ps7

