#include <SDL3/SDL_stdinc.h>

#include "graphics.h"
#include "shader.h"

SDL_GPUSampler* Sampler::createSampler(const Window& win, SDL_GPUSamplerCreateInfo& sampleInfo)
{
    SDL_GPUSampler* sample = SDL_CreateGPUSampler(win.getGPU(), &sampleInfo);

    if(sample == NULL) return nullptr;
    return sample;
}

template <> Store<SDL_GPUSampler>::~Store()
{
    Logger::debug(std::string("Deconstructing Store of ") + typeid(SDL_GPUSampler*).name());
    for(std::pair<std::string, SDL_GPUSampler*> value : *this)
    {
        Logger::debug(std::string(" - ") + value.first);
    }
    this->clear();
    Logger::debug(std::string("Deconstructed Store of ") + typeid(SDL_GPUSampler*).name());
}

SDL_GPUGraphicsPipeline* Pipeline::createPipeline(const Window& win, const std::string& vertShader = "default", const std::string& fragShader = "default")
{
	SDL_GPUColorTargetBlendState colorBlend;
	SDL_zero(colorBlend);

	colorBlend.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
	colorBlend.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlend.color_blend_op = SDL_GPU_BLENDOP_ADD;

	colorBlend.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
	colorBlend.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlend.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
	colorBlend.enable_blend = true;

	SDL_GPUColorTargetDescription colorTargetDescription[]
	{
		{
			.format = SDL_GetGPUSwapchainTextureFormat(win.getGPU(), win.getWin()),
			.blend_state = colorBlend,
		}
	};

	// Create the pipeline
	SDL_GPUGraphicsPipelineTargetInfo targetInfo;
	SDL_zero(targetInfo);
	targetInfo.color_target_descriptions = colorTargetDescription;
	targetInfo.num_color_targets = 1;

	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo;
	SDL_zero(pipelineCreateInfo);
	pipelineCreateInfo.vertex_shader = Shader::get(vertShader);
	pipelineCreateInfo.fragment_shader = Shader::get(fragShader);
	pipelineCreateInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
	pipelineCreateInfo.target_info = targetInfo;

    SDL_GPUGraphicsPipeline* pipe = SDL_CreateGPUGraphicsPipeline(win.getGPU(), &pipelineCreateInfo);

    if(pipe == NULL) return nullptr;
	return pipe;
};

template <> Store<SDL_GPUGraphicsPipeline>::~Store()
{
    Logger::debug(std::string("Deconstructing Store of ") + typeid(SDL_GPUGraphicsPipeline*).name());
    for(std::pair<std::string, SDL_GPUGraphicsPipeline*> value : *this)
    {
        Logger::debug(std::string(" - ") + value.first);
    }
    this->clear();
    Logger::debug(std::string("Deconstructed Store of ") + typeid(SDL_GPUGraphicsPipeline*).name());
}