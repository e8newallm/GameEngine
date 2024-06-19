#project(lua LANGUAGES CXX)
#add_compile_definitions(MAKE_LIB)
#add_library(lua STATIC lua/onelua.c)

add_subdirectory(Catch2)