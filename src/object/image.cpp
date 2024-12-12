#include "image.h"
#include "texture_base.h"

Image::Image(SDL_Rect body, Texture_base* texture, Uint8 layer) :
    Object(body, texture)
    , layer(layer)
{
}