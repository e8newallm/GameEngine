#ifndef SPRITEMAPDATA_H
#define SPRITEMAPDATA_H

#include <SDL3/SDL_gpu.h>
#include <map>
#include <vector>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/schema.h>

#include <SDL3/SDL.h>

#include "tools/packager/packager.h"

struct Sprite
{
    SDL_GPUTexture* texture;
    std::string textureName;
    SDL_FRect position;
};

struct Animation
{
    double FPS;
    std::vector<std::string> sprites;
};

struct CurrentFrame
{
    double elapsedTime = 0.0;
    Uint32 frame = 0;
};

class SpriteMapData
{
    public:
        SpriteMapData();

        void loadFromFile(SDL_GPUDevice* gpu, const char* configLocation);
        void loadFromPackage(SDL_GPUDevice* gpu, PackageManager* package, const char* spriteConfig);
        void loadFromString(SDL_GPUDevice* gpu, const char* spriteConfig, const char* source = "string config");
        void save(const char* spriteConfig);
        std::string serialise();

        PackageManager* package;

        std::map<std::string, SDL_GPUTexture*> textures;
        std::map<std::string, Sprite> sprites;
        std::map<std::string, Animation> animations;
};

#endif