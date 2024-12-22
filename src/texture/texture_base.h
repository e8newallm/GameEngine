#ifndef TEXTURE_BASE_H
#define TEXTURE_BASE_H

#include <SDL2/SDL_render.h>

class Texture_base
{
    public:
        virtual void update(double deltaT) = 0;
        virtual void draw(SDL_Renderer* rend, SDL_Rect* bodyPos) = 0;
};

#endif