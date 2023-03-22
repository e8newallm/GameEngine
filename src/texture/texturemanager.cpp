#include "texturemanager.h"
#include <filesystem>

void TextureManager::load(std::string fileLocation)
{
    
}

void TextureManager::unload(std::string textureName)
{

}

SDL_Texture* TextureManager::operator[](std::string name)
{
    return (textures.find(name)->second);
}