#ifndef SPRITEMAPMANAGER_H
#define SPRITEMAPMANAGER_H

#include <map>

#include "spritemapdata.h"

class SpriteMapManager
{
    public:
        SpriteMapManager(SDL_Renderer* rend) : rend(rend) {};
        void load(std::string fileLocation);
        void unload(std::string spriteMapName);
        SpriteMapData& operator[](std::string name);

    private:
        std::map<std::string, SpriteMapData*> spriteMaps;
        SDL_Renderer* rend;
};

SpriteMapManager* getSpriteMapManager(SDL_Renderer* rend = nullptr) { static SpriteMapManager man(rend); return &man; }

#endif