#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Object
{
    public:
        Object(SDL_Rect body, SDL_Texture* texture);
        Object() : Object((SDL_Rect){0, 0, 0, 0}, NULL) {};
        ~Object();

        virtual void draw(SDL_Renderer* rend);
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
        SDL_Texture* tex;
};

#endif