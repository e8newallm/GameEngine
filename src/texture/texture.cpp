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

void Texture::draw(SDL_Renderer* rend, SDL_Rect* bodyPos)
{
    SDL_RenderCopyEx(rend, texture, &texturePosition, bodyPos, 0.0, NULL, SDL_FLIP_NONE);
}


template <> Store<SDL_Texture>::~Store()
{
    std::cout << "Deconstructing Store of " << typeid(SDL_Texture*).name() << "\r\n" << std::flush;
    for(std::pair<std::string, SDL_Texture*> value : *this)
    {
        std::cout << " - " << value.first << "\r\n";
        SDL_DestroyTexture(value.second);
    }
    std::cout << "Deconstructed Store of " << typeid(SDL_Texture*).name() << "\r\n" << std::flush;
}