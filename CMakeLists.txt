cmake_minimum_required(VERSION 3.25)

function(version_define ProjectName Major Minor Patch)
    add_compile_definitions(${ProjectName}_MAJOR=${Major})
    add_compile_definitions(${ProjectName}_MINOR=${Minor})
    add_compile_definitions(${ProjectName}_PATCH=${Patch})
endfunction()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

include(ProcessorCount)
ProcessorCount(N)
set(CMAKE_BUILD_PARALLEL_LEVEL ${N})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(DCMAKE_EXPORT_COMPILE_COMMANDS true)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(Catch2 3 REQUIRED)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(SDL3_image REQUIRED)

add_subdirectory(src)
add_subdirectory(test)