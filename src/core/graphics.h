#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <string>

#include "datastore.h"
#include "window.h"

struct ShaderObjData
{
    SDL_Rect position;
};

class Sampler : public DataStore<SDL_GPUSampler, Sampler>
{
    public:
        static SDL_GPUSampler* createSampler(Window& win, SDL_GPUSamplerCreateInfo& sampleInfo);
};

class Pipeline : public DataStore<SDL_GPUGraphicsPipeline, Pipeline>
{
    public:
        static SDL_GPUGraphicsPipeline* createPipeline(Window& win, const std::string vertShader, const std::string fragShader);
};
#endif