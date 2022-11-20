find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

file(GLOB src 
        keystate.cpp
        object/object.cpp
        object/physicsobject.cpp
        object/player.cpp
    )
file(GLOB headers
        object
    )

add_library(GameEngine STATIC ${src})
target_link_libraries(GameEngine PUBLIC ${SDL2_LIBRARIES} SDL2_image)
target_include_directories(GameEngine PUBLIC . ${headers} ${SDL2_INCLUDE_DIRS})

add_executable(GameEngineExample ${src} main.cpp)
target_link_libraries(GameEngineExample PUBLIC ${SDL2_LIBRARIES} SDL2_image GameEngine)
target_include_directories(GameEngineExample PUBLIC . ${headers} ${SDL2_INCLUDE_DIRS})
add_custom_command(TARGET GameEngineExample PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/tex/ $<TARGET_FILE_DIR:GameEngineExample>/tex)
