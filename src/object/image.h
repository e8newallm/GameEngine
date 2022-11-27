#ifndef IMAGE_H
#define IMAGE_H

#include "object.h"

class Context;

class Image : public Object
{
    public:
        Image(SDL_Rect body, SDL_Texture* texture, bool isForeground);

        bool isForeground() { return foreground; };
        
    private:
        bool foreground;
};

#endif