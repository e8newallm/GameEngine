#include <iostream>

#include "spritemap.h"

SpriteMap::SpriteMap(SDL_Renderer* rend, const char* texturePath, SDL_Point spriteDimensions) :
    Texture(rend, texturePath)
    , spriteDimensions(spriteDimensions)
    , currentPos({0, 0, 150, 150})
{
    
}

TexRequest SpriteMap::getTexture()
{
    return {texture, currentPos};
}

void SpriteMap::nextSprite()
{
    currentPos.x += 150;
    //std::cout << "currentPos: " << currentPos.x << ":" << currentPos.y << " " << currentPos.h << ":" << currentPos.w << "\r\n";
    if(currentPos.x >= 7*150)
    {
        currentPos.x = 0;
        currentPos.y += 150;

        if(currentPos.y >= 3*150)
        {
            currentPos.y = 0;
        }
    }
}