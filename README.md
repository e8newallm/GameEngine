# GameEngine

A library based on SDL3 for developing 2D games. 

## Dependencies
 - [SDL3](https://github.com/libsdl-org/SDL)
 - [SDL3_image](https://github.com/libsdl-org/SDL_image)
 - [Catch2](https://github.com/catchorg/Catch2)
 - [Rapidjson](https://github.com/Tencent/rapidjson)

## Build

Before building the library, make sure all of the above dependencies are installed.

1. Clone the repository
```sh
git clone https://github.com/e8newallm/gameengine.git
```
2. Pull all submodules used by the library
```sh
git submodule update --init --recursive
```
3. Build the library
```sh
./build.sh
```

## Example

A simple example of how to use the library can be found [here](https://github.com/e8newallm/GameEngine-example).

## Roadmap
TODO: Add roadmap