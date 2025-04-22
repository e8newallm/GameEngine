#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <memory>
#include <string>

#include "datastore.h"
#include "window.h"

namespace GameEng
{
    class GPUDevice
    {
        public:
            static void GPUDeviceInit(SDL_GPUShaderFormat flags = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL);
            static void GPUDeviceDeinit();
            static SDL_GPUDevice* getGPU();

            GPUDevice() = default;

        private:
            static GPUDevice device;

            SDL_GPUDevice* gpu = nullptr;
    };

    /**
     * \brief A structure for holding the data blob the shader will require.
     * 
     */
    struct ShaderObjData
    {
        void* data; ///< A pointer to the data in memory.
        size_t size; ///< The size of the data.
    };

    /**
     * \brief A class for wrapping SDL_GPUSampler for use with the DataStore in the class. Also contains functions for creating and storing all created SDL samplers.
     *
     */
    class Sampler : public DataStore<Sampler, Sampler>
    {
        public:
            /**
             * \brief Construct a new Sampler wrapper
             *
             * \param sampler The SDL_GPUSampler* to be wrapped
             */
            explicit Sampler(SDL_GPUSampler* sampler) { this->sampler = sampler; };

            /**
             * \brief Destroy the Sampler wrapper as well as the SDL_GPUSampler
             * 
             */
            ~Sampler()
            {
                if(sampler != nullptr)
                {
                    SDL_ReleaseGPUSampler(GPUDevice::getGPU(), sampler);
                }
            }

            /**
             * \brief Create a Sampler object.
             *
             * \param win The window the sampler will be used for.
             * \param sampleInfo A structure that details how the sampler will operate.
             * \return SDL_GPUSampler* The sampler object.
             *
             * Note: Any other window sharing the SDL_GPUDevice can also use this sampler.
             */
            static std::shared_ptr<Sampler> createSampler(const Window& win, SDL_GPUSamplerCreateInfo& sampleInfo);

            /**
             * \brief Get the SDL_GPUSampler from the object.
             *
             * \return SDL_GPUSampler* The SDL pipeline object.
             */
             SDL_GPUSampler* getSampler() { return sampler; };

             Sampler(const Sampler&) = delete;

        private:
            SDL_GPUSampler* sampler;
    };

    /**
     * \brief A class for wrapping SDL_GPUGraphicsPipeline for use with the DataStore in the class. Also contains functions for creating and storing all created SDL Pipelines.
     *
     */
    class Pipeline : public DataStore<Pipeline, Pipeline>
    {
        public:
            /**
             * \brief Construct a new Pipeline wrapper
             *
             * \param sampler The SDL_GPUGraphicsPipeline* to be wrapped
             */
            explicit Pipeline(SDL_GPUGraphicsPipeline* pipeline) { this->pipeline = pipeline; };

            /**
             * \brief Destroy the Pipeline wrapper as well as the SDL_GPUPipeline
             * 
             */
             ~Pipeline()
             {
                if(pipeline != nullptr)
                {
                    SDL_ReleaseGPUGraphicsPipeline(GPUDevice::getGPU(), pipeline);
                }
             }

            /**
             * \brief Create a Pipeline object using shaders saved in the Shader DataStore.
             *
             * \param win The window the pipline will be used for.
             * \param vertShader The name of the vertex shader.
             * \param fragShader The name of the fragment shader.
             * \return SDL_GPUGraphicsPipeline* The created pipeline.
             */
            static std::shared_ptr<Pipeline> createPipeline(const Window& win, const std::string& vertShader, const std::string& fragShader);

            /**
             * \brief Get the SDL_GPUGraphicsPipeline from the object.
             *
             * \return SDL_GPUGraphicsPipeline* The SDL pipeline object.
             */
            SDL_GPUGraphicsPipeline* getPipeline() { return pipeline; };

            Pipeline(const Pipeline&) = delete;
        private:
            SDL_GPUGraphicsPipeline* pipeline;
    };
}

#endif