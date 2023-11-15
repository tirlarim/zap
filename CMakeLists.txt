cmake_minimum_required(VERSION 3.26)
project(zap-2023-8748 C)

set(CMAKE_C_STANDARD 11)
add_compile_definitions(DEBUG_FLAG)
add_executable(zap-2023-8748 ./ps3/main.c ./ps3/hangman.c ./ps3/morse.c)
