cmake_minimum_required(VERSION 3.5.1)
project(dbe C)

set(CMAKE_C_STANDARD 99)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

add_executable(dbe main.c)
target_link_libraries(dbe lmdb)