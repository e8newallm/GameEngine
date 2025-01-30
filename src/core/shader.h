#ifndef SHADER_H
#define SHADER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

#include <vector>

#include "datastore.h"
#include "packager.h"

class Shader : public DataStore<SDL_GPUShader, Shader>
{
    public:
        SDL_GPUShader* LoadShaderFromArray(
            SDL_GPUDevice* device,
            std::string shaderFilename,
            std::vector<uint8_t> code,
            Uint32 samplerCount,
            Uint32 uniformBufferCount,
            Uint32 storageBufferCount,
            Uint32 storageTextureCount
        );

        static SDL_GPUShader* LoadShaderFromFile(
            SDL_GPUDevice* device,
            std::string shaderFilename,
            Uint32 samplerCount,
            Uint32 uniformBufferCount,
            Uint32 storageBufferCount,
            Uint32 storageTextureCount
        );

        static SDL_GPUShader* createShader(
            SDL_GPUDevice* device,
            std::vector<uint8_t> code,
            std::string filename,
            Uint32 samplerCount,
            Uint32 uniformBufferCount,
            Uint32 storageBufferCount,
            Uint32 storageTextureCount
        );
};

#endif