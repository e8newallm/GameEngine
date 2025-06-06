#include "spritemap.h"
#include "graphics.h"
#include "spritemapdata.h"
#include "texture.h"
#include "tools/packager/packager.h"
#include <memory>

namespace GameEng
{
SpriteMap::SpriteMap(const char* configLocation)
    : currentAnimation(nullptr), currentFrame({.elapsedTime = 0.0, .frame = 0}), currentSprite(nullptr),
      data(new SpriteMapData())
{
    std::string storeName = std::string(configLocation);
    if(!exists(storeName))
    {
        data = std::make_shared<SpriteMapData>();
        data->loadFromFile(configLocation);
        add(data, storeName);
    }
    else
    {
        data = get(storeName);
    }
}

SpriteMap::SpriteMap(const Packager::PackageManager* package, const char* path)
    : currentAnimation(nullptr), currentFrame({.elapsedTime = 0.0, .frame = 0}), currentSprite(nullptr)
{
    std::string storeName = package->getPackageName() + ":" + std::string(path);
    if(!exists(storeName))
    {
        data = std::make_shared<SpriteMapData>();
        data->loadFromPackage(package, path);
        add(data, storeName);
    }
    else
    {
        data = get(storeName);
    }
}

SpriteMap::SpriteMap(SpriteMapData* spriteData)
    : currentAnimation(nullptr), currentFrame({.elapsedTime = 0.0, .frame = 0}), currentSprite(nullptr),
      data(spriteData)
{
}

SDL_FRect SpriteMap::getUV()
{
    if(currentSprite == nullptr)
    {
        return {0, 0, 0, 0};
    }

    SDL_FRect pos = currentSprite->position;
    const GPUTexture* tex = currentSprite->texture.get();
    return {.x = (pos.x / static_cast<float>(tex->getWidth())),
            .y = pos.y / static_cast<float>(tex->getHeight()),
            .w = (pos.x + pos.w) / static_cast<float>(tex->getWidth()),
            .h = (pos.y + pos.h) / static_cast<float>(tex->getHeight())};
}

void SpriteMap::update(double deltaT)
{
    static const float milliseconds = 1000.0F;
    if(currentAnimation != nullptr)
    {
        currentFrame.elapsedTime += deltaT;
        while(currentFrame.elapsedTime > milliseconds / currentAnimation->FPS)
        {
            currentFrame.elapsedTime -= milliseconds / currentAnimation->FPS;
            currentFrame.frame++;
            if(currentFrame.frame > currentAnimation->sprites.size() - 1)
            {
                currentFrame = {.elapsedTime = 0.0, .frame = 0};
                currentAnimation = nullptr;
                break;
            }
            setAnimationSprite(currentAnimation->sprites[currentFrame.frame]);
        }
    }
}

void SpriteMap::draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass)
{
    // static auto samp = Sampler::get("default");
    (void)world;
    if(currentSprite != nullptr)
    {
        SDL_GPUTextureSamplerBinding sampleBinding;
        SDL_zero(sampleBinding);
        sampleBinding.texture = currentSprite->texture->getTex();
        sampleBinding.sampler = Sampler::get("default")->getSampler();

        SDL_BindGPUGraphicsPipeline(renderPass, Pipeline::get("default")->getPipeline());
        SDL_BindGPUFragmentSamplers(renderPass, 0, &sampleBinding, 1);
        SDL_BindGPUVertexStorageBuffers(renderPass, 0, &buffer, 1);
        SDL_DrawGPUPrimitives(renderPass, 6, 1, 0, 0); // NOLINT
    }
}

void SpriteMap::setSprite(const std::string& name)
{
    if(!data->sprites.contains(name))
    {
        return;
    }

    currentSprite = &data->sprites.find(name)->second;
    currentFrame = {.elapsedTime = 0.0, .frame = 0};
    currentAnimation = nullptr;
}

void SpriteMap::setAnimationSprite(const std::string& name)
{
    if(!data->sprites.contains(name))
    {
        return;
    }

    currentSprite = &data->sprites.find(name)->second;
}

void SpriteMap::startAnimation(const std::string& animation)
{
    if(!data->animations.contains(animation))
    {
        return;
    }

    currentAnimation = &data->animations.find(animation)->second;
    currentFrame = {.elapsedTime = 0.0, .frame = 0};
    setAnimationSprite(currentAnimation->sprites[currentFrame.frame]);
}
} // namespace GameEng