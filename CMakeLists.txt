cmake_minimum_required(VERSION 3.26)
project(zap-2023-8748 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
#add_compile_definitions(DEBUG_FLAG)
#add_compile_definitions(HANGMAN)
add_compile_definitions(MORSE)
add_executable(zap-2023-8748 ./ps3/main.c ./ps3/hangman.c ./ps3/morse.c
        ps3/utils.c)
