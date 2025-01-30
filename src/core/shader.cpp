#include <SDL3/SDL.h>
#include <cstdint>

#include "shader.h"

SDL_GPUShader* Shader::createShader(
	SDL_GPUDevice* device,
	std::vector<uint8_t> code,
    std::string filename,
	Uint32 samplerCount,
	Uint32 uniformBufferCount,
	Uint32 storageBufferCount,
	Uint32 storageTextureCount
) {
	SDL_GPUShaderStage stage;
	if (SDL_strstr(filename.c_str(), ".vert"))
	{
		stage = SDL_GPU_SHADERSTAGE_VERTEX;
	}
	else if (SDL_strstr(filename.c_str(), ".frag"))
	{
		stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	}
	else
	{
		SDL_Log("Invalid shader stage!");
		return nullptr;
	}

	char fullPath[256];
	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char* entrypoint;

	if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%s/%s.spv", ".", filename.c_str());
		format = SDL_GPU_SHADERFORMAT_SPIRV;
		entrypoint = "main";
	} else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%s/%s.msl", ".", filename.c_str());
		format = SDL_GPU_SHADERFORMAT_MSL;
		entrypoint = "main0";
	} else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%s/%s.dxil", ".", filename.c_str());
		format = SDL_GPU_SHADERFORMAT_DXIL;
		entrypoint = "main";
	} else {
		return nullptr;
	}

	SDL_GPUShaderCreateInfo shaderInfo = {
		.code_size = code.size(),
		.code = static_cast<Uint8*>(code.data()),
		.entrypoint = entrypoint,
		.format = format,
		.stage = stage,
		.num_samplers = samplerCount,
		.num_storage_textures = storageTextureCount,
		.num_storage_buffers = storageBufferCount,
		.num_uniform_buffers = uniformBufferCount,
	};
	SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
	if (shader == NULL)
	{
		std::cout <<"Failed to create shader!\r\n";
		return nullptr;
	}

	return shader;
}

SDL_GPUShader* Shader::LoadShaderFromFile(
	SDL_GPUDevice* device,
	std::string shaderFilename,
	Uint32 samplerCount,
	Uint32 uniformBufferCount,
	Uint32 storageBufferCount,
	Uint32 storageTextureCount
) {
	size_t codeSize;
    uint8_t* codeRaw = (uint8_t*)SDL_LoadFile(shaderFilename.c_str(), &codeSize);
	if (codeRaw == NULL)
	{
		SDL_Log("Failed to load shader from disk! %s", shaderFilename.c_str());
		return nullptr;
	}

    std::vector<uint8_t> code;
    code.assign(codeRaw, codeRaw + codeSize);

    return createShader(device, code, shaderFilename, samplerCount, uniformBufferCount, storageBufferCount, storageTextureCount);
}

SDL_GPUShader* Shader::LoadShaderFromArray(
	SDL_GPUDevice* device,
	std::string shaderFilename,
    std::vector<uint8_t> code,
	Uint32 samplerCount,
	Uint32 uniformBufferCount,
	Uint32 storageBufferCount,
	Uint32 storageTextureCount
) {
    return createShader(device, code, shaderFilename, samplerCount, uniformBufferCount, storageBufferCount, storageTextureCount);
}