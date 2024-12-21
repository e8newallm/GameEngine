cmake_minimum_required(VERSION 3.25)

project(Packager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Packager STATIC packager.cpp ../../core/geerror.cpp)

set_target_properties(Packager
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tools/"
)

find_package(ZLIB)
target_link_libraries(Packager ZLIB::ZLIB)
target_include_directories(Packager PUBLIC . ../../{core,utility})

project(PackagerBin LANGUAGES CXX)
add_executable(PackagerBin main.cpp)
target_link_libraries(PackagerBin ZLIB::ZLIB Packager)