#include "graphics.h"
#include "shader.h"

SDL_GPUSampler* Sampler::createSampler(Window& win, SDL_GPUSamplerCreateInfo& sampleInfo)
{
    SDL_GPUSampler* sample = SDL_CreateGPUSampler(win.getGPU(), &sampleInfo);

    if(sample == NULL) return nullptr;
    return sample;
}

SDL_GPUGraphicsPipeline* Pipeline::createPipeline(Window& win, const std::string vertShader = "default", const std::string fragShader = "default")
{
	SDL_GPUColorTargetBlendState colorBlend {
		.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
		.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
		.color_blend_op = SDL_GPU_BLENDOP_ADD,

		.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
		.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
		.alpha_blend_op = SDL_GPU_BLENDOP_ADD,
		.enable_blend = true,
	};

	SDL_GPUColorTargetDescription colorTargetDescription[]
	{
	{
			.format = SDL_GetGPUSwapchainTextureFormat(win.getGPU(), win.getWin()),
			.blend_state = colorBlend,
		}
	};

	// Create the pipeline
	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
		.vertex_shader = Shader::get(vertShader),
		.fragment_shader = Shader::get(fragShader),
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,

		.target_info = {
			.color_target_descriptions = colorTargetDescription,
			.num_color_targets = 1,
		},
	};

    SDL_GPUGraphicsPipeline* pipe = SDL_CreateGPUGraphicsPipeline(win.getGPU(), &pipelineCreateInfo);

    if(pipe == NULL) return nullptr;
	return pipe;
};