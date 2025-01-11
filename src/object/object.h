#ifndef OBJECT_H
#define OBJECT_H

#include <SDL3/SDL.h>

#include "texture_base.h"
#include "world.h"

class Object
{
    public:
        Object(SDL_Rect body, Texture_base* texture);
        Object() : Object({0, 0, 0, 0}, nullptr) {};
        virtual ~Object();

        virtual void draw(World* world, double deltaT = 0);
        virtual void draw(World* world, SDL_Rect* bodyPos, double deltaT);
        virtual void update(double deltaTime);

        virtual void move(double x, double y);
        virtual void moveDelta(double x, double y);

        virtual void resize(double height, double width);
        virtual void resizeDelta(double height, double width);

        SDL_Rect* getBody() {return &body;};

    protected:
        SDL_Rect body;
        Texture_base* tex;
};

#endif