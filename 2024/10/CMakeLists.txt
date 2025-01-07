cmake_minimum_required(VERSION 3.27)

project(aoc_10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

