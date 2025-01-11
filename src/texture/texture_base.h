#ifndef TEXTURE_BASE_H
#define TEXTURE_BASE_H

#include <SDL3/SDL.h>

#include "world.h"


class Texture_base
{
    public:
        virtual void update(double deltaT) = 0;
        virtual void draw(World* world, SDL_Rect* bodyPos) = 0;
        virtual ~Texture_base() {};
};

#endif