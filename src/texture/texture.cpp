#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

#include "texture.h"

Texture::Texture() :
    texture{nullptr},
    texturePosition{0, 0, 0, 0}
{
}

Texture::Texture(std::string name) :
    texture{Texture::get(name)},
    texturePosition{0, 0, 0, 0}
{
    SDL_QueryTexture(texture, NULL, NULL, &texturePosition.w, &texturePosition.h);
}

TexRequest Texture::getTexture()
{
    return {texture, texturePosition};
}