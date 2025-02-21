#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include "object.h"

class Context;

class Image : public Object
{
    public:
        Image(SDL_Rect body, const std::string& texture);

        ShaderObjData predraw() override;
};

#endif