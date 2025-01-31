#ifndef TEXTURE_BASE_H
#define TEXTURE_BASE_H

#include <SDL3/SDL.h>

#include "world.h"
#include "graphics.h"

class Texture_base
{
    public:
        virtual void update(double deltaT) = 0;
        virtual void draw(World* world, SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass, ShaderObjData objData) = 0;
        virtual ~Texture_base() {};
};

#endif