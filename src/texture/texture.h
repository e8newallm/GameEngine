#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

#include "datastore.h"

struct TexRequest
{
    SDL_Texture* texture;
    SDL_Rect position;
};

class Texture : public DataStore<SDL_Texture>
{
    public:
        Texture();
        Texture(std::string name);

        virtual TexRequest getTexture();
        virtual void update(double deltaT) {(void) deltaT;};

    protected:
        SDL_Texture* texture;
        SDL_Rect texturePosition;
};

#endif