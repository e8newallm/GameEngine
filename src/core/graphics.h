#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <string>

#include "datastore.h"
#include "window.h"

namespace GameEng
{
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
     * \brief A class for creating and storing all created SDL Samplers.
     *
     */
    class Sampler : public DataStore<SDL_GPUSampler, Sampler>
    {
        public:
            /**
             * \brief Create a Sampler object.
             *
             * \param win The window the sampler will be used for.
             * \param sampleInfo A structure that details how the sampler will operate.
             * \return SDL_GPUSampler* The sampler object.
             *
             * Note: Any other window sharing the SDL_GPUDevice can also use this sampler.
             */
            static SDL_GPUSampler* createSampler(const Window& win, SDL_GPUSamplerCreateInfo& sampleInfo);
    };

    template <> Store<SDL_GPUSampler>::~Store();

    /**
     * \brief A class for creating and storing all created SDL Pipelines.
     *
     */
    class Pipeline : public DataStore<SDL_GPUGraphicsPipeline, Pipeline>
    {
        public:
            /**
             * \brief Create a Pipeline object using shaders saved in the Shader DataStore.
             *
             * \param win The window the pipline will be used for.
             * \param vertShader The name of the vertex shader.
             * \param fragShader The name of the fragment shader.
             * \return SDL_GPUGraphicsPipeline* The created pipeline.
             */
            static SDL_GPUGraphicsPipeline* createPipeline(const Window& win, const std::string& vertShader, const std::string& fragShader);
    };

    template <> Store<SDL_GPUGraphicsPipeline>::~Store();
}

#endif