#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>

class TextureManager
{
    public:
        TextureManager(SDL_Renderer* rend) : rend(rend) {};
        void load(std::string fileLocation);
        void unload(std::string textureName);
        SDL_Texture* operator[](std::string name);

    private:
        std::map<std::string, SDL_Texture*> textures;
        SDL_Renderer* rend;
}

TextureManager* getTextureManager(SDL_Renderer* rend = nullptr) { static TextureManager man(rend); return &man; }

#endif