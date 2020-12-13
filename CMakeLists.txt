cmake_minimum_required(VERSION 3.5.1)
project(dbe C)

set(CMAKE_C_STANDARD 99)

add_executable(dbe main.c)
target_link_libraries(dbe lmdb)