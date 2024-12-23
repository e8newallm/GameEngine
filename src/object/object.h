#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "texture_base.h"

class Object
{
    public:
        Object(SDL_Rect body, Texture_base* texture);
        Object() : Object({0, 0, 0, 0}, nullptr) {};
        virtual ~Object();

        virtual void draw(SDL_Renderer* rend, double deltaT = 0);
        virtual void draw(SDL_Renderer* rend, SDL_Rect* bodyPos, double deltaT);
        virtual void update(double deltaTime);

        virtual void move(double x, double y);
        virtual void moveInstantly(double x, double y);
        virtual void moveDelta(double x, double y);
        virtual void moveDeltaInstantly(double x, double y);

        virtual void resize(double height, double width);
        virtual void resizeDelta(double height, double width);

        SDL_Rect* getBody() {return &body;};

    protected:
        SDL_Rect body;
        SDL_FRect updateBody;
        Texture_base* tex;
};

#endif