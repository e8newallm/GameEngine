#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL.h>

#include "texture_base.h"
#include "datastore.h"

class Texture : public Texture_base, public DataStore<SDL_GPUTexture, Texture>
{
    public:
        Texture();
        explicit Texture(const std::string& name);

        virtual void update(double deltaT) override {(void) deltaT;};
        virtual void draw(World* world, SDL_Rect* bodyPos) override;

    protected:
        SDL_GPUTexture* texture;
        SDL_FRect texturePosition;
};

template <> Store<SDL_GPUTexture>::~Store();

SDL_GPUTexture* uploadTexture(SDL_GPUDevice* gpu, SDL_Surface* surf, std::string filename);
#endif