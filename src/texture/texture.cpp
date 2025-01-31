#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <string>

#include "logging.h"
#include "texture.h"
#include "graphics.h"

Texture::Texture() :
    texture{nullptr},
    texturePosition{0, 0, 0, 0}
{
}

Texture::Texture(const std::string& name) :
    texture{Texture::get(name)},
    texturePosition{0, 0, (float)0, (float)0}
{
}

void Texture::draw(World* world, SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass, ShaderObjData objData)
{
    //SDL_RenderTexture(world->getGPU(), texture, &texturePosition, &bPos);

    SDL_PushGPUVertexUniformData(cmdbuf, 2, &objData, sizeof(ShaderObjData));
    SDL_BindGPUGraphicsPipeline(renderPass, Pipeline::get("default"));
    SDL_BindGPUFragmentSamplers(renderPass, 0, &(SDL_GPUTextureSamplerBinding){ .texture = texture, .sampler = Sampler::get("default") }, 1);
    SDL_DrawGPUPrimitives(renderPass, 6, 1, 0, 0);
}

template <> Store<SDL_GPUTexture>::~Store()
{
    Logger::debug(std::string("Deconstructing Store of ") + typeid(SDL_Texture*).name());
    for(std::pair<std::string, SDL_GPUTexture*> value : *this)
    {
        Logger::debug(std::string(" - ") + value.first);
    }
    this->clear();
    Logger::debug(std::string("Deconstructed Store of ") + typeid(SDL_Texture*).name());
}

SDL_GPUTexture* uploadTexture(SDL_GPUDevice* gpu, SDL_Surface* surf, std::string filename)
{
    SDL_Surface* convertSurf = SDL_ConvertSurface(surf, SDL_PIXELFORMAT_ABGR8888);

    SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(gpu);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

    SDL_GPUTextureCreateInfo texInfo;
    SDL_zero(texInfo);
    texInfo.type = SDL_GPU_TEXTURETYPE_2D;
    texInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    texInfo.width = convertSurf->w;
    texInfo.height = convertSurf->h;
    texInfo.layer_count_or_depth = 1;
    texInfo.num_levels = 1;
    texInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    SDL_GPUTexture* tex = SDL_CreateGPUTexture(gpu, &texInfo);
    SDL_SetGPUTextureName(gpu, tex, filename.c_str());

    SDL_GPUTransferBufferCreateInfo texBuffer;
    SDL_zero(texBuffer);
    texBuffer.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    texBuffer.size = convertSurf->h * convertSurf->pitch;
    SDL_GPUTransferBuffer* textureTransferBuffer = SDL_CreateGPUTransferBuffer(gpu, &texBuffer);

    void* textureTransferPtr = SDL_MapGPUTransferBuffer(gpu,
                                            textureTransferBuffer, false);
    SDL_memcpy(textureTransferPtr, convertSurf->pixels, texBuffer.size);
    SDL_UnmapGPUTransferBuffer(gpu, textureTransferBuffer);

    SDL_GPUTextureTransferInfo texTrans;
    SDL_zero(texTrans);
    texTrans.transfer_buffer = textureTransferBuffer;
    texTrans.offset = 0;

    SDL_GPUTextureRegion texRegion;
    SDL_zero(texRegion);
    texRegion.texture = tex;
    texRegion.w = convertSurf->w;
    texRegion.h = convertSurf->h;
    texRegion.d = 1;
    SDL_UploadToGPUTexture(copyPass, &texTrans, &texRegion, false);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(uploadCmdBuf);
    SDL_ReleaseGPUTransferBuffer(gpu, textureTransferBuffer);

    return tex;
}