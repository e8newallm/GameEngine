#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Object
{
    public:
        Object(double x, double y, double height, double width, SDL_Texture* texture);
        Object() : Object(0.0f, 0.0f, 0.0f, 0.0f, NULL) {};
        ~Object();

        void draw(SDL_Renderer* rend);
        void update(double deltaTime);

        void move(double x, double y);
        void moveDelta(double x, double y);

        void velocity(double x, double y);
        void velocityDelta(double x, double y);

        void resize(double height, double width);
        void resizeDelta(double height, double width);

    private:
        SDL_Rect body;
        SDL_FRect updateBody;
        SDL_FPoint currentVelocity;
        SDL_Texture* tex;
};

#endif