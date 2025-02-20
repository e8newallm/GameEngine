#ifndef OBJECT_H
#define OBJECT_H

#include <SDL3/SDL.h>

#include "graphics.h"
#include "texture_base.h"
#include "world.h"

class Object
{
    public:
        Object(SDL_Rect body, Texture_base* texture);
        Object() : Object({0, 0, 0, 0}, nullptr) {};
        virtual ~Object();

        virtual ShaderObjData predraw() = 0;

        virtual void draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass, double deltaT = 0);
        virtual void update(double deltaTime, World& world);
        virtual void runPhysics(double deltaTime, World& world);

        virtual void move(double x, double y);
        virtual void moveDelta(double x, double y);

        virtual void resize(double height, double width);
        virtual void resizeDelta(double height, double width);

        virtual const SDL_Rect* getBody() const { return &body; };

    protected:
        SDL_Rect body;
        Texture_base* tex;
};

#endif