#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <string>

#include "datastore.h"
#include "window.h"

struct ShaderObjData
{
    void* data;
    size_t size;
};

class Sampler : public DataStore<SDL_GPUSampler, Sampler>
{
    public:
        static SDL_GPUSampler* createSampler(const Window& win, SDL_GPUSamplerCreateInfo& sampleInfo);
};

template <> Store<SDL_GPUSampler>::~Store();

class Pipeline : public DataStore<SDL_GPUGraphicsPipeline, Pipeline>
{
    public:
        static SDL_GPUGraphicsPipeline* createPipeline(const Window& win, const std::string& vertShader, const std::string& fragShader);
};
template <> Store<SDL_GPUGraphicsPipeline>::~Store();

#endif