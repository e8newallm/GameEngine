#ifndef TEXTURE_BASE_H
#define TEXTURE_BASE_H

#include <SDL3/SDL.h>

#include "world.h"
#include "graphics.h"

class Texture_base
{
    public:
        virtual void update(double deltaT) = 0;
        virtual void draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass) = 0;
        virtual SDL_FRect getUV() { return SDL_FRect{0.0f, 0.0f, 1.0f, 1.0f}; };
        virtual ~Texture_base() {};
};

#endif