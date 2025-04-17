#include <SDL3/SDL_stdinc.h>
#include <memory>

#include "graphics.h"
#include "shader.h"

namespace GameEng
{
	std::shared_ptr<Sampler> Sampler::createSampler(const Window& win, SDL_GPUSamplerCreateInfo& sampleInfo)
	{
		SDL_GPUSampler* sample = SDL_CreateGPUSampler(win.getGPU(), &sampleInfo);

		if(sample == NULL) return nullptr;
		return std::make_shared<Sampler>(Sampler(sample));
	}

	std::shared_ptr<Pipeline> Pipeline::createPipeline(const Window& win, const std::string& vertShader = "default", const std::string& fragShader = "default")
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
		pipelineCreateInfo.vertex_shader = Shader::get(vertShader)->getShader();
		pipelineCreateInfo.fragment_shader = Shader::get(fragShader)->getShader();
		pipelineCreateInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
		pipelineCreateInfo.target_info = targetInfo;

		SDL_GPUGraphicsPipeline* pipe = SDL_CreateGPUGraphicsPipeline(win.getGPU(), &pipelineCreateInfo);

		if(pipe == NULL) return nullptr;
		return std::make_shared<Pipeline>(Pipeline(pipe));
	};
}