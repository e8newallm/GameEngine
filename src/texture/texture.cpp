#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <memory>
#include <string>

#include "graphics.h"
#include "texture.h"

namespace GameEng
{
Texture::Texture() : texture{nullptr}, texturePosition{0, 0, 0, 0}
{
}

Texture::Texture(const std::string& name) : texture{Texture::get(name)}, texturePosition{0, 0, (float)0, (float)0}
{
}

void Texture::draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass)
{
    (void)world;
    static SDL_GPUSampler* samp = Sampler::get("default")->getSampler();
    SDL_GPUTextureSamplerBinding sampleBinding;
    SDL_zero(sampleBinding);
    sampleBinding.texture = texture->getTex();
    sampleBinding.sampler = samp;

    SDL_BindGPUGraphicsPipeline(renderPass, Pipeline::get("default")->getPipeline());
    SDL_BindGPUFragmentSamplers(renderPass, 0, &sampleBinding, 1);
    SDL_BindGPUVertexStorageBuffers(renderPass, 0, &buffer, 1);
    SDL_DrawGPUPrimitives(renderPass, 6, 1, 0, 0); // NOLINT
}

std::shared_ptr<GPUTexture> createTexture(SDL_GPUDevice* gpu, SDL_Surface* surf, const std::string& filename)
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

    void* textureTransferPtr = SDL_MapGPUTransferBuffer(gpu, textureTransferBuffer, false);
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

    return std::make_shared<GPUTexture>(tex, surf->w, surf->h);
}
} // namespace GameEng