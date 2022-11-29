#ifndef SPRITEMAP_H
#define SPRITEMAP_H

#include "texture.h"

class SpriteMap : public Texture
{
    public:
        SpriteMap(SDL_Renderer* rend, const char* texturePath, SDL_Point spriteDimensions);
        TexRequest getTexture() override;
        void nextSprite();

    private:
        SDL_Point spriteDimensions;
        SDL_Rect currentPos;
};

#endif