find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(RapidJSON REQUIRED)

file(GLOB src 
        keystate.cpp
        mousestate.cpp
        context.cpp
        view.cpp
        object/object.cpp
        object/physicsobject.cpp
        object/image.cpp
        texture/texture.cpp
        texture/spritemap.cpp
    )

file(GLOB headers
        object
        texture
        schema
    )

add_library(GameEngine STATIC ${src})
target_link_libraries(GameEngine PUBLIC ${SDL2_LIBRARIES} SDL2_image)
target_include_directories(GameEngine PUBLIC . ${headers} ${SDL2_INCLUDE_DIRS})

add_custom_command(TARGET GameEngine PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/schema/ $<TARGET_FILE_DIR:GameEngine>/schema)