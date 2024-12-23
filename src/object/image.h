#ifndef IMAGE_H
#define IMAGE_H

#include "object.h"
#include "texture.h"

class Context;

class Image : public Object
{
    public:
        Image(SDL_Rect body, std::string texture, Uint8 layer);

        void draw(SDL_Renderer* rend, double deltaT = 0);
        void draw(SDL_Renderer* rend, SDL_Rect* bodyPos, double deltaT);

        Uint8 getLayer() { return layer; };
        
    private:
        Uint8 layer;
};

#endif