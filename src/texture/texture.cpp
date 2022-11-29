#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "texture.h"

Texture::Texture(SDL_Renderer* rend, const char* texturePath)
{
    texture = SDL_CreateTextureFromSurface(rend, IMG_Load(texturePath));
}

TexRequest Texture::getTexture()
{
    SDL_Point size;
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    return {texture, {0, 0, size.x, size.y}};
}

