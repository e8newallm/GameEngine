add_subdirectory(example)
add_subdirectory(Catch2)

project(UnitTests LANGUAGES CXX)

file(GLOB_RECURSE jsonfiles "json/*")

add_custom_target(TestJSON
  ALL
  DEPENDS ${jsonfiles}
)

add_custom_command(
    TARGET TestJSON POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_CURRENT_SOURCE_DIR}/json
      ${CMAKE_BINARY_DIR}/bin/json
)

add_executable(UnitTests ${jsonfiles} test.cpp)
target_link_libraries(UnitTests PRIVATE Catch2::Catch2WithMain GameEngine)
target_include_directories(UnitTests PUBLIC . ${headers} ${SDL2_INCLUDE_DIRS} ${CMAKE_BINARY_DIR}/src Catch2/src/catch2)
add_dependencies(UnitTests SchemaLoader)