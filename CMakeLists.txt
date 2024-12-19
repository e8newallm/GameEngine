cmake_minimum_required(VERSION 3.25)
project(Project)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic)

include(ProcessorCount)
ProcessorCount(N)
set(CMAKE_BUILD_PARALLEL_LEVEL ${N})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(Catch2 3 REQUIRED)

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(test)