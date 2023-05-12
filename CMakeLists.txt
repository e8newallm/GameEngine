cmake_minimum_required(VERSION 3.25)
project(Project)

include(ProcessorCount)
ProcessorCount(N)
set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
message("number of processors: "  ${CMAKE_BUILD_PARALLEL_LEVEL})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(test)