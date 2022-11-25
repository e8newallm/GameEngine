add_subdirectory(example)
add_subdirectory(Catch2)

project(UnitTests LANGUAGES CXX)

add_executable(UnitTests test.cpp)
target_link_libraries(UnitTests PRIVATE Catch2::Catch2WithMain GameEngine)
target_include_directories(UnitTests PUBLIC . ${headers} ${SDL2_INCLUDE_DIRS} Catch2/src/catch2)