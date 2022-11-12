cmake_minimum_required(VERSION 3.15)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

project(GameProj LANGUAGES CXX)
add_subdirectory(src)

#option(BP_BUILD_TESTS "Build tests" ON)
#if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BP_BUILD_TESTS)
#    enable_testing()
#    add_subdirectory(test)
#endif()