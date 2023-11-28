cmake_minimum_required(VERSION 3.26)
project(zap-2023-8748 C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_C_FLAGS "-Werror=vla")
set(CMAKE_C_FLAGS "-Wall -march=native")

set(currentProblemSet 4)
add_compile_definitions(DEBUG_FLAG)

if (currentProblemSet EQUAL 1)
    add_executable(ps1 ./ps1/task_1.c) #ps1
elseif(currentProblemSet EQUAL 2)
    add_executable(ps2 ./ps2/arrays.c) #ps2
elseif(currentProblemSet EQUAL 3)
    add_executable(ps3 ./ps3/main.c ./ps3/hangman.c ./ps3/morse.c) #ps3
elseif(currentProblemSet EQUAL 4)
    add_executable(ps4 ./ps4/main.c ./ps4/ballsortpuzzle.c ./ps4/c4.c) #ps4
    target_link_libraries(ps4 curses) #ps6
elseif(currentProblemSet EQUAL 5)
    add_executable(ps5 ./ps5/qr.c) #ps5
elseif(currentProblemSet EQUAL 6)
    add_executable(ps6 ps6/main.c ps6/life/engine.c ps6/life/graphic.c ps6/utils/utils.c) #ps6
    target_link_libraries(ps6 curses)
endif ()
