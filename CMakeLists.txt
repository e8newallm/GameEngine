cmake_minimum_required(VERSION 3.25)

function(version_define ProjectName Major Minor Patch)
    add_compile_definitions(${ProjectName}_MAJOR=${Major})
    add_compile_definitions(${ProjectName}_MINOR=${Minor})
    add_compile_definitions(${ProjectName}_PATCH=${Patch})
endfunction()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SDLSHADERCROSS_VENDORED ON)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(Catch2 3 REQUIRED)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_subdirectory(src)
add_subdirectory(extern)
add_subdirectory(test)
