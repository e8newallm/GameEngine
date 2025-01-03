#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

#include "texture_base.h"
#include "datastore.h"

class Texture : public Texture_base, public DataStore<SDL_Texture, Texture>
{
    public:
        Texture();
        Texture(std::string name);

        virtual void update(double deltaT) override {(void) deltaT;};
        virtual void draw(World* world, SDL_Rect* bodyPos) override;

    protected:
        SDL_Texture* texture;
        SDL_Rect texturePosition;
};

template <> Store<SDL_Texture>::~Store();
#endif