#include <SDL3/SDL.h>

#include "logging.h"
#include "texture.h"

Texture::Texture() :
    texture{nullptr},
    texturePosition{0, 0, 0, 0}
{
}

Texture::Texture(const std::string& name) :
    texture{Texture::get(name)},
    texturePosition{0, 0, (float)texture->w, (float)texture->h}
{

}

void Texture::draw(World* world, SDL_Rect* bodyPos)
{
    SDL_FRect bPos;
    SDL_RectToFRect(bodyPos, &bPos);
    SDL_RenderTexture(world->getRend(), texture, &texturePosition, &bPos);
}


template <> Store<SDL_Texture>::~Store()
{
    Logger::debug(std::string("Deconstructing Store of ") + typeid(SDL_Texture*).name());
    for(std::pair<std::string, SDL_Texture*> value : *this)
    {
        Logger::debug(std::string(" - ") + value.first);
    }
    this->clear();
    Logger::debug(std::string("Deconstructed Store of ") + typeid(SDL_Texture*).name());
}