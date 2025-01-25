#include <SDL3/SDL.h>
#include <string>

#include "logging.h"
#include "texture.h"

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

void Texture::draw(World* world, SDL_Rect* bodyPos)
{
    SDL_FRect bPos;
    SDL_RectToFRect(bodyPos, &bPos);
    //SDL_RenderTexture(world->getGPU(), texture, &texturePosition, &bPos);
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
    SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(gpu);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

    SDL_GPUTextureCreateInfo texInfo;
    SDL_zero(texInfo);
    texInfo.type = SDL_GPU_TEXTURETYPE_2D;
    texInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    texInfo.width = surf->w;
    texInfo.height = surf->h;
    texInfo.layer_count_or_depth = 1;
    texInfo.num_levels = 1;
    texInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    SDL_GPUTexture* tex = SDL_CreateGPUTexture(gpu, &texInfo);
    SDL_SetGPUTextureName(gpu, tex, filename.c_str());
    Logger::message("SDL_SetGPUTextureName");
    SDL_GPUTransferBufferCreateInfo texBuffer;
    SDL_zero(texBuffer);
    texBuffer.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    texBuffer.size = surf->w * surf->h * 4;
    SDL_GPUTransferBuffer* textureTransferBuffer = SDL_CreateGPUTransferBuffer(gpu, &texBuffer);
    Logger::message("textureTransferBuffer");
    void* textureTransferPtr = SDL_MapGPUTransferBuffer(gpu,
                                            textureTransferBuffer, false);
    Logger::message("TEST " + std::to_string(surf->w * surf->h * 4));
    Logger::message("Pointer " + std::to_string((long long)surf->pixels));
    Logger::message("textureTransferPtr " + std::to_string((long long)textureTransferPtr));
    SDL_memcpy(textureTransferPtr, surf->pixels, surf->w * surf->h);
    Logger::message("TEST");
    SDL_UnmapGPUTransferBuffer(gpu, textureTransferBuffer);
    Logger::message("TEST");

    SDL_GPUTextureTransferInfo texTrans;
    SDL_zero(texTrans);
    texTrans.transfer_buffer = textureTransferBuffer;
    texTrans.offset = 0;

    SDL_GPUTextureRegion texRegion;
    SDL_zero(texRegion);
    texRegion.texture = tex;
    texRegion.w = surf->w;
    texRegion.h = surf->h;
    texRegion.d = 1;
    SDL_UploadToGPUTexture(copyPass, &texTrans, &texRegion, false);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(uploadCmdBuf);
    SDL_ReleaseGPUTransferBuffer(gpu, textureTransferBuffer);

    return tex;
}