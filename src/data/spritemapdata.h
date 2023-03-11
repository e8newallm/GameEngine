#ifndef SPRITEMAPDATA_H
#define SPRITEMAPDATA_H

#include <map>
#include <vector>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/schema.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Sprite
{
    SDL_Texture* texture;
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
        SpriteMapData(SDL_Renderer* rend, const char* spriteConfig);
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, Sprite> sprites;
        std::map<std::string, Animation> animations;
};

#endif