#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include <cstdint>
#include <cstring>
#include <memory>

#include "logging.h"
#include "shader.h"

namespace GameEng
{
std::vector<uint8_t> Shader::buildShader(const std::string& filename, const std::vector<uint8_t>& source)
{
    std::string sourceStr = std::string(source.begin(), source.end());
    SDL_ShaderCross_HLSL_Info hlslInfo;
    hlslInfo.source = sourceStr.c_str();
    hlslInfo.entrypoint = "main";
    hlslInfo.include_dir = nullptr;
    hlslInfo.defines = nullptr;
    hlslInfo.enable_debug = false;
    hlslInfo.name = filename.c_str();
    hlslInfo.props = 0;

    if(SDL_strstr(filename.c_str(), ".vert") != nullptr)
    {
        hlslInfo.shader_stage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX;
    }
    else if(SDL_strstr(filename.c_str(), ".frag") != nullptr)
    {
        hlslInfo.shader_stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;
    }

    size_t size;
    void* data = SDL_ShaderCross_CompileSPIRVFromHLSL(&hlslInfo, &size);
    if(size == 0)
    {
        Logger::error("Failed to build shader: ", SDL_GetError(), "\r\n");
    }

    std::vector<uint8_t> result(size);
    std::memcpy(result.data(), data, size);
    SDL_free(data);
    return result;
}

std::shared_ptr<Shader> Shader::createShader(SDL_GPUDevice* gpu, const std::string& filename,
                                             const std::vector<uint8_t>& code, Uint32 samplerCount,
                                             Uint32 uniformBufferCount, Uint32 storageBufferCount,
                                             Uint32 storageTextureCount)
{
    SDL_GPUShaderStage stage;
    if(SDL_strstr(filename.c_str(), ".vert") != nullptr)
    {
        stage = SDL_GPU_SHADERSTAGE_VERTEX;
    }
    else if(SDL_strstr(filename.c_str(), ".frag") != nullptr)
    {
        stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    }
    else
    {
        SDL_Log("Invalid shader stage!");
        return nullptr;
    }

    SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(gpu);
    SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
    const char* entrypoint;

    if((backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) != 0U)
    {
        format = SDL_GPU_SHADERFORMAT_SPIRV;
        entrypoint = "main";
    }
    else if((backendFormats & SDL_GPU_SHADERFORMAT_MSL) != 0U)
    {
        format = SDL_GPU_SHADERFORMAT_MSL;
        entrypoint = "main0";
    }
    else if((backendFormats & SDL_GPU_SHADERFORMAT_DXIL) != 0U)
    {
        format = SDL_GPU_SHADERFORMAT_DXIL;
        entrypoint = "main";
    }
    else
    {
        return nullptr;
    }

    SDL_GPUShaderCreateInfo shaderInfo;
    SDL_zero(shaderInfo);
    shaderInfo.code_size = code.size();
    shaderInfo.code = const_cast<uint8_t*>(code.data());
    shaderInfo.entrypoint = entrypoint;
    shaderInfo.format = format;
    shaderInfo.stage = stage;
    shaderInfo.num_samplers = samplerCount;
    shaderInfo.num_storage_textures = storageTextureCount;
    shaderInfo.num_storage_buffers = storageBufferCount;
    shaderInfo.num_uniform_buffers = uniformBufferCount;

    SDL_GPUShader* shader = SDL_CreateGPUShader(gpu, &shaderInfo);
    if(shader == nullptr)
    {
        Logger::error("Failed to create shader: ", SDL_GetError(), "\r\n");
        return nullptr;
    }

    return std::make_shared<Shader>(shader, gpu);
}

std::shared_ptr<Shader> Shader::LoadShaderFromFile(SDL_GPUDevice* gpu, const std::string& shaderFilename,
                                                   Uint32 samplerCount, Uint32 uniformBufferCount,
                                                   Uint32 storageBufferCount, Uint32 storageTextureCount)
{
    size_t codeSize;
    uint8_t* codeRaw = static_cast<uint8_t*>(SDL_LoadFile(shaderFilename.c_str(), &codeSize));
    if(codeRaw == nullptr)
    {
        Logger::error("Failed to load shader from disk! ", shaderFilename);
        return nullptr;
    }

    std::vector<uint8_t> code;
    code.assign(codeRaw, codeRaw + codeSize);

    return createShader(gpu, shaderFilename, code, samplerCount, uniformBufferCount, storageBufferCount,
                        storageTextureCount);
}

std::shared_ptr<Shader> Shader::LoadShaderFromArray(SDL_GPUDevice* gpu, const std::string& shaderFilename,
                                                    const std::vector<uint8_t>& code, Uint32 samplerCount,
                                                    Uint32 uniformBufferCount, Uint32 storageBufferCount,
                                                    Uint32 storageTextureCount)
{
    return createShader(gpu, shaderFilename, code, samplerCount, uniformBufferCount, storageBufferCount,
                        storageTextureCount);
}
} // namespace GameEng