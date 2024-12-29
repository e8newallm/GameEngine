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

MACRO(package_gen folder file)
IF(NOT PACKAGER_INT)
    set(PACKAGER_INT 1)
ENDIF()
message("Running packager on folder " ${folder} " to make package " ${file} "TEST: " ${PACKAGER_INT})
add_custom_target(DataPackager_${PACKAGER_INT}
  ALL
  COMMAND PackagerBin ${folder} ${file}
)
MATH(EXPR PACKAGER_INT "${PACKAGER_INT}+1")
ENDMACRO()
