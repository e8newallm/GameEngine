#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include <iostream>

#include "texture.h"

Texture::Texture(SDL_Renderer* rend)
{
    texture = SDL_CreateTextureFromSurface(rend, nullptr);
}

Texture::Texture(SDL_Renderer* rend, const char* texturePath)
{
    if(texturePath == nullptr)
        texture = SDL_CreateTextureFromSurface(rend, nullptr);
    else
        texture = SDL_CreateTextureFromSurface(rend, IMG_Load(texturePath));
}

Texture::Texture(SDL_Renderer* rend, std::vector<uint8_t> data)
{
    SDL_RWops* dataBuffer = SDL_RWFromMem(data.data(), data.size());
    texture = SDL_CreateTextureFromSurface(rend, IMG_Load_RW(dataBuffer, 1));
}

TexRequest Texture::getTexture()
{
    SDL_Point size;
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    return {texture, {0, 0, size.x, size.y}};
}

