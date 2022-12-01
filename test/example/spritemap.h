#ifndef SPRITEMAP_H
#define SPRITEMAP_H

#include <map>
#include <rapidjson/document.h>

#include "texture.h"

struct Sprite
{
    SDL_Texture* texture;
    SDL_Rect position;
};

class SpriteMap : public Texture
{
    public:
        SpriteMap(SDL_Renderer* rend, const char* texturePath, const char* spriteConfig);
        TexRequest getTexture() override;
        void setSprite(char* name);

    private:
        std::map<const char*, SDL_Texture*> textures;
        std::map<const char*, Sprite> sprites;
        std::map<const char*, Sprite>::const_iterator currentSprite;
};

#endif