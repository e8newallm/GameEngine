#ifndef IMAGE_H
#define IMAGE_H

#include "object.h"
#include "texture.h"

class Context;

class Image : public Object
{
    public:
        Image(SDL_Rect body, Texture* texture, Uint8 layer);

        Uint8 getLayer() { return layer; };
        
    private:
        Uint8 layer;
};

#endif