cmake_minimum_required(VERSION 3.24)
project(untitled C)

set(CMAKE_C_STANDARD 17)

add_executable(untitled main.c)

set(CMAKE_C_FLAGS -pthread)