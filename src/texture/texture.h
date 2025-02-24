#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL.h>

#include "texture_base.h"
#include "datastore.h"

struct GPUTexture
{
    SDL_GPUTexture* tex;
    int width, height;
};

class Texture : public Texture_base, public DataStore<GPUTexture, Texture>
{
    public:
        Texture();
        explicit Texture(const std::string& name);

        virtual void update(double deltaT) override {(void) deltaT;};
        virtual void draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass) override;

    protected:
        GPUTexture* texture;
        SDL_FRect texturePosition;
};

SDL_GPUTexture* uploadTexture(SDL_GPUDevice* gpu, SDL_Surface* surf, const std::string& filename);
#endif