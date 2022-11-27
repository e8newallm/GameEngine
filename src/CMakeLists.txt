find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

file(GLOB src 
        keystate.cpp
        mousestate.cpp
        context.cpp
        view.cpp
        object/object.cpp
        object/physicsobject.cpp
        object/image.cpp
    )

file(GLOB headers
        ./object
    )

add_library(GameEngine STATIC ${src})
target_link_libraries(GameEngine PUBLIC ${SDL2_LIBRARIES} SDL2_image)
target_include_directories(GameEngine PUBLIC . ${headers} ${SDL2_INCLUDE_DIRS})