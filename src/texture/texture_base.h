#ifndef TEXTURE_BASE_H
#define TEXTURE_BASE_H

#include <SDL2/SDL_render.h>

struct TexRequest
{
    SDL_Texture* texture;
    SDL_Rect position;
};

class Texture_base
{
    public:
        virtual TexRequest getTexture() = 0;
        virtual void update(double deltaT) = 0;
};

#endif