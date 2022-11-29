#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct TexRequest
{
    SDL_Texture* texture;
    SDL_Rect position;
};

class Texture
{
    public:
        Texture(SDL_Renderer* rend, const char* texturePath);
        virtual TexRequest getTexture();

    protected:
        SDL_Texture* texture;
};

#endif