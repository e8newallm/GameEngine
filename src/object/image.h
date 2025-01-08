#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include "object.h"

class Context;

class Image : public Object
{
    public:
        Image(SDL_Rect body, const std::string& texture);

        void draw(World* world, double deltaT = 0) override;
        void draw(World* world, SDL_Rect* bodyPos, double deltaT) override;
};

#endif