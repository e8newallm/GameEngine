#include "spritemap.h"
#include "spritemapdata.h"
#include "texture_base.h"
#include "tools/packager/packager.h"

SpriteMap::SpriteMap(SDL_GPUDevice* gpu, const char* spriteConfig) :
    currentAnimation(nullptr)
    , currentFrame({0.0, 0})
    , currentSprite(nullptr)
    , data(new SpriteMapData())
{
    std::string storeName = std::string(spriteConfig);
    if(!exists(storeName))
    {
        data = new SpriteMapData();
        data->loadFromFile(gpu, spriteConfig);
        add(data, storeName);
    }
    else
    {
        data = get(storeName);
    }
}

SpriteMap::SpriteMap(SDL_GPUDevice* gpu, PackageManager* package, const char* path) :
    currentAnimation(nullptr)
    , currentFrame({0.0, 0})
    , currentSprite(nullptr)
{
    std::string storeName = package->getPackageName() + ":" + std::string(path);
    if(!exists(storeName))
    {
        data = new SpriteMapData();
        data->loadFromPackage(gpu, package, path);
        add(data, storeName);
    }
    else
    {
        data = get(storeName);
    }

}

SpriteMap::SpriteMap(SpriteMapData* spriteData) :
    currentAnimation(nullptr)
    , currentFrame({0.0, 0})
    , currentSprite(nullptr)
    , data(spriteData)
{
}

SDL_FRect SpriteMap::getUV()
{
    return Texture_base::getUV();
    //currentSprite->texture
    //currentSprite->position
}

void SpriteMap::update(double deltaT)
{
    if(currentAnimation != nullptr)
    {
        currentFrame.elapsedTime += deltaT;
        while(currentFrame.elapsedTime > 1000.0f/currentAnimation->FPS)
        {
            currentFrame.elapsedTime -= 1000.0f/currentAnimation->FPS;
            currentFrame.frame++;
            if(currentFrame.frame > currentAnimation->sprites.size()-1)
            {
                currentFrame = {0.0, 0};
                currentAnimation = nullptr;
                break;
            }
            else
            {
                setAnimationSprite(currentAnimation->sprites[currentFrame.frame]);
            }
        }

    }
}

void SpriteMap::draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass)
{
    if(currentSprite != nullptr)
    {
        //SDL_RenderTexture(world->getGPU(), currentSprite->texture, &currentSprite->position, &bPos);
        SDL_BindGPUGraphicsPipeline(renderPass, Pipeline::get("default"));
        SDL_BindGPUFragmentSamplers(renderPass, 0, &(SDL_GPUTextureSamplerBinding){ .texture = currentSprite->texture, .sampler = Sampler::get("default") }, 1);
        SDL_BindGPUVertexStorageBuffers(renderPass, 0, &buffer, 1);
        SDL_DrawGPUPrimitives(renderPass, 6, 1, 0, 0);
    }
}

void SpriteMap::setSprite(const std::string& name)
{
    if(data->sprites.find(name) == data->sprites.end())
        return;

    currentSprite = &data->sprites.find(name)->second;
    currentFrame = {0.0, 0};
    currentAnimation = nullptr;
}

void SpriteMap::setAnimationSprite(const std::string& name)
{
    if(data->sprites.find(name) == data->sprites.end())
        return;
    
    currentSprite = &data->sprites.find(name)->second;

}

void SpriteMap::startAnimation(const std::string& animation)
{
    if(data->animations.find(animation) == data->animations.end())
        return;

    currentAnimation = &data->animations.find(animation)->second;
    currentFrame = {0.0, 0};
    setAnimationSprite(currentAnimation->sprites[currentFrame.frame]);
}