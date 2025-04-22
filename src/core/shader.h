#ifndef SHADER_H
#define SHADER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

#include <memory>
#include <vector>
#include <iostream>

#include "datastore.h"

namespace GameEng
{
    /**
     * \brief Class for wrapping SDL_GPUShader* for use with the DataStore in the class. Also has functions for creating SDL_GPUShader objects.
     *
     */
    class Shader : public DataStore<Shader, Shader>
    {
        public:
            /**
             * \brief Construct a new Shader wrapper
             *
             * \param sampler The SDL_GPUShader* to be wrapped
             */
            Shader(SDL_GPUShader* shader, SDL_GPUDevice* newGPU) :
            shader(shader)
            ,gpu(newGPU)
            {};

            /**
             * \brief Destroy the Shader object
             * 
             */
            ~Shader()
            {
                SDL_ReleaseGPUShader(gpu, shader);
            };

            /**
             * \brief Create a Shader object from an vector array
             *
             * \param gpu The SDL_GPUDevice* to load the shader into.
             * \param shaderFilename The filename of the shader (Used for parsing the format).
             * \param code The shader code to be used.
             * \param samplerCount The number of samplers defined in the shader.
             * \param uniformBufferCount The number of uniform buffers defined in the shader.
             * \param storageBufferCount The number of storage buffers defined in the shader.
             * \param storageTextureCount The number of storage textures defined in the shader.
             * \return std::shared_ptr<Shader> The resulting shader saved in the GPU.
             */
            static std::shared_ptr<Shader> LoadShaderFromArray(
                SDL_GPUDevice* gpu,
                const std::string& shaderFilename,
                const std::vector<uint8_t>& code,
                Uint32 samplerCount,
                Uint32 uniformBufferCount,
                Uint32 storageBufferCount,
                Uint32 storageTextureCount
            );

            /**
             * \brief Create a Shader object from af file
             *
             * \param gpu The SDL_GPUDevice* to load the shader into.
             * \param shaderFilename The filename of the shader (Used for parsing the format).
             * \param samplerCount The number of samplers defined in the shader.
             * \param uniformBufferCount The number of uniform buffers defined in the shader.
             * \param storageBufferCount The number of storage buffers defined in the shader.
             * \param storageTextureCount The number of storage textures defined in the shader.
             * \return std::shared_ptr<Shader> The resulting shader saved in the GPU.
             */
            static std::shared_ptr<Shader> LoadShaderFromFile(
                SDL_GPUDevice* gpu,
                const std::string& shaderFilename,
                Uint32 samplerCount,
                Uint32 uniformBufferCount,
                Uint32 storageBufferCount,
                Uint32 storageTextureCount
            );

            /**
             * \brief Create a SDL_GPUShader* object
             *
             * \param gpu The SDL_GPUDevice* to load the shader into.
             * \param filename The filename of the shader (Used for parsing the format).
             * \param code The shader code to be used.
             * \param samplerCount The number of samplers defined in the shader.
             * \param uniformBufferCount The number of uniform buffers defined in the shader.
             * \param storageBufferCount The number of storage buffers defined in the shader.
             * \param storageTextureCount The number of storage textures defined in the shader.
             * \return std::shared_ptr<Shader> The resulting shader saved in the GPU.
             */
            static std::shared_ptr<Shader> createShader(
                SDL_GPUDevice* gpu,
                const std::string& filename,
                const std::vector<uint8_t>& code,
                Uint32 samplerCount,
                Uint32 uniformBufferCount,
                Uint32 storageBufferCount,
                Uint32 storageTextureCount
            );

            /**
             * \brief Get the SDL_GPUShader from the object.
             *
             * \return SDL_GPUShader* The SDL shader object.
             */
            SDL_GPUShader* getShader() { return shader; };

        private:
            SDL_GPUShader* shader;
            SDL_GPUDevice* gpu;
    };
}

#endif