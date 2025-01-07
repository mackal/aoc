cmake_minimum_required(VERSION 3.27)

project(aoc_3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES main.cpp)

find_package(scn CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

set(LIBS scn::scn)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})

