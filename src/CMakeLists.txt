find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

file(GLOB src "*.cpp")

add_executable(GameProj ${src})
target_link_libraries(GameProj PUBLIC ${SDL2_LIBRARIES} SDL2_image)
target_include_directories(GameProj PUBLIC ${SDL2_INCLUDE_DIRS})

add_custom_command(TARGET GameProj PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/tex/ $<TARGET_FILE_DIR:GameProj>/tex)