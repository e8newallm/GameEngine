project(GameEngineExample LANGUAGES CXX)

add_executable(GameEngineExample main.cpp player.cpp)
target_link_libraries(GameEngineExample PUBLIC ${SDL2_LIBRARIES} SDL2_image GameEngine)
target_include_directories(GameEngineExample PUBLIC ${SDL2_INCLUDE_DIRS})
add_custom_command(TARGET GameEngineExample PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/tex/ $<TARGET_FILE_DIR:GameEngineExample>/tex)
