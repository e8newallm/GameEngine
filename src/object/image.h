#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include "object.h"

class Context;

class Image : public Object
{
    public:
        Image(SDL_Rect body, std::string texture);

        void draw(SDL_Renderer* rend, double deltaT = 0);
        void draw(SDL_Renderer* rend, SDL_Rect* bodyPos, double deltaT);
};

#endif