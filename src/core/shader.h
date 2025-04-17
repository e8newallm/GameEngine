#ifndef SHADER_H
#define SHADER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

#include <vector>

#include "datastore.h"

namespace GameEng
{
    /**
     * \brief Class for creating SDL_GPUShader objects (Contains a DataStore for storage also).
     *
     */
    class Shader : public DataStore<SDL_GPUShader, Shader>
    {
        public:
            /**
             * \brief
             *
             * \param device The SDL_GPUDevice* to load the shader into.
             * \param shaderFilename The filename of the shader (Used for parsing the format).
             * \param code The shader code to be used.
             * \param samplerCount The number of samplers defined in the shader.
             * \param uniformBufferCount The number of uniform buffers defined in the shader.
             * \param storageBufferCount The number of storage buffers defined in the shader.
             * \param storageTextureCount The number of storage textures defined in the shader.
             * \return SDL_GPUShader* The resulting shader saved in the GPU.
             */
            static SDL_GPUShader* LoadShaderFromArray(
                SDL_GPUDevice* device,
                const std::string& shaderFilename,
                const std::vector<uint8_t>& code,
                Uint32 samplerCount,
                Uint32 uniformBufferCount,
                Uint32 storageBufferCount,
                Uint32 storageTextureCount
            );

            /**
             * \brief
             * 
             * \param device The SDL_GPUDevice* to load the shader into.
             * \param shaderFilename The filename of the shader (Used for parsing the format).
             * \param samplerCount The number of samplers defined in the shader.
             * \param uniformBufferCount The number of uniform buffers defined in the shader.
             * \param storageBufferCount The number of storage buffers defined in the shader.
             * \param storageTextureCount The number of storage textures defined in the shader.
             * \return SDL_GPUShader* The resulting shader saved in the GPU.
             */
            static SDL_GPUShader* LoadShaderFromFile(
                SDL_GPUDevice* device,
                const std::string& shaderFilename,
                Uint32 samplerCount,
                Uint32 uniformBufferCount,
                Uint32 storageBufferCount,
                Uint32 storageTextureCount
            );

            /**
             * \brief Create a SDL_GPUShader* object
             *
             * \param device The SDL_GPUDevice* to load the shader into.
             * \param filename The filename of the shader (Used for parsing the format).
             * \param code The shader code to be used.
             * \param samplerCount The number of samplers defined in the shader.
             * \param uniformBufferCount The number of uniform buffers defined in the shader.
             * \param storageBufferCount The number of storage buffers defined in the shader.
             * \param storageTextureCount The number of storage textures defined in the shader.
             * \return SDL_GPUShader* The resulting shader saved in the GPU.
             */
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