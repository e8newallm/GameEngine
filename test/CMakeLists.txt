project(UnitTests LANGUAGES CXX)

file(GLOB_RECURSE jsonfiles "json/*")

add_custom_target(TestJSON
  ALL
  DEPENDS ${jsonfiles}
)

add_custom_command(
    TARGET TestJSON POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_CURRENT_SOURCE_DIR}/testfiles
      ${CMAKE_BINARY_DIR}/bin/testfiles
)

add_executable(UnitTests ${jsonfiles} test.cpp packager.cpp)
target_link_libraries(UnitTests PRIVATE Catch2::Catch2WithMain GameEngine Packager)
target_include_directories(UnitTests PUBLIC . ../src ${SDL2_INCLUDE_DIRS} Catch2::Catch2)
add_dependencies(UnitTests SchemaLoader)