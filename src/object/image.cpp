#include "image.h"
#include "context.h"
#include "texture.h"

Image::Image(SDL_Rect body, Texture* texture, bool isForeground) :
    Object(body, texture)
    , foreground(isForeground)
{
}