#include "image.h"
#include "context.h"

Image::Image(SDL_Rect body, SDL_Texture* texture, bool isForeground) :
    Object(body, texture)
    , foreground(isForeground)
{
}