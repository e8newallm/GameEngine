#include "image.h"
#include "texture.h"

Image::Image(SDL_Rect body, Texture* texture, Uint8 layer) :
    Object(body, texture)
    , layer(layer)
{
}