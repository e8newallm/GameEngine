find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(RapidJSON REQUIRED)

add_subdirectory(tools)

file(GLOB schemalist
    ${CMAKE_CURRENT_SOURCE_DIR}/schema/*.json
)

add_custom_target(SchemaGeneration
  ALL
  COMMAND SchemaLoader ${CMAKE_BINARY_DIR} ${schemalist}
  DEPENDS ${schemalist} tools/schemaloader/schemaloader.cpp SchemaLoader
)

project(GameEngine LANGUAGES CXX)

version_define(GAMEENGINE 0 0 1)

file(GLOB GAMEENGINE_HEADERS
    object
    texture
    texture/spritemap
    schema
    data
    core
    utility
)

file(GLOB src
    core/*.cpp
    utility/*.cpp
    object/*.cpp
    texture/*.cpp
    texture/*/*.cpp
)

if(DEBUG)
    file(GLOB debugsrc debug/*.cpp)
    list(APPEND src ${debugsrc})
endif()

add_library(GameEngine STATIC ${src})
target_link_libraries(GameEngine PUBLIC ${SDL3_LIBRARIES} SDL3_image lua Packager)
target_include_directories(GameEngine PUBLIC . ${GAMEENGINE_HEADERS} ${CMAKE_BINARY_DIR})
add_dependencies(GameEngine SchemaGeneration)

if(DEBUG)
    target_compile_definitions(GameEngine PUBLIC DEBUG)
endif()