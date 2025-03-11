#ifndef SHADER_H
#define SHADER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

#include <vector>

#include "datastore.h"

namespace GameEng
{
    class Shader : public DataStore<SDL_GPUShader, Shader>
    {
        public:
            static SDL_GPUShader* LoadShaderFromArray(
                SDL_GPUDevice* device,
                const std::string& shaderFilename,
                const std::vector<uint8_t>& code,
                Uint32 samplerCount,
                Uint32 uniformBufferCount,
                Uint32 storageBufferCount,
                Uint32 storageTextureCount
            );

            static SDL_GPUShader* LoadShaderFromFile(
                SDL_GPUDevice* device,
                const std::string& shaderFilename,
                Uint32 samplerCount,
                Uint32 uniformBufferCount,
                Uint32 storageBufferCount,
                Uint32 storageTextureCount
            );

            static SDL_GPUShader* createShader(
                SDL_GPUDevice* device,
                const std::string& filename,
                const std::vector<uint8_t>& code,
                Uint32 samplerCount,
                Uint32 uniformBufferCount,
                Uint32 storageBufferCount,
                Uint32 storageTextureCount
            );
    };

    template <> Store<SDL_GPUShader>::~Store();
}

#endif