#ifndef SPRITEMAPDATA_H
#define SPRITEMAPDATA_H

#include <map>
#include <vector>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/schema.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "tools/packager/packager.h"

struct Sprite
{
    SDL_Texture* texture;
    std::string textureName;
    SDL_Rect position;
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

        void loadFromFile(SDL_Renderer* rend, const char* configLocation);
        void loadFromPackage(SDL_Renderer* rend, PackageManager* package, const char* spriteConfig);
        void loadFromString(SDL_Renderer* rend, const char* spriteConfig, const char* source = "string config");
        void save(const char* spriteConfig);
        std::string serialise();

        PackageManager* package;

        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, Sprite> sprites;
        std::map<std::string, Animation> animations;
};

#endif