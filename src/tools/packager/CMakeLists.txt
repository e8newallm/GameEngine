cmake_minimum_required(VERSION 3.5)

project(Packager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Packager STATIC packager.cpp)

set_target_properties(Packager
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tools/"
)

find_package(ZLIB)
target_link_libraries(Packager ZLIB::ZLIB)

project(PackagerBin LANGUAGES CXX)
add_executable(PackagerBin main.cpp)
target_link_libraries(PackagerBin ZLIB::ZLIB Packager)