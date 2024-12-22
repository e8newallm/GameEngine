#include "spritemap.h"
#include "spritemapdata.h"
#include "tools/packager/packager.h"

SpriteMap::SpriteMap(SDL_Renderer* rend, const char* spriteConfig) :
    currentAnimation(nullptr)
    , currentFrame({0.0, 0})
    , currentSprite(nullptr)
    , data(new SpriteMapData())
{
    std::string storeName = std::string(spriteConfig);
    if(!exists(storeName))
    {
        data = new SpriteMapData();
        data->loadFromFile(rend, spriteConfig);
        add(data, storeName);
    }
    else
    {
        data = get(storeName);
    }
}

SpriteMap::SpriteMap(SDL_Renderer* rend, PackageManager* package, const char* path) :
    currentAnimation(nullptr)
    , currentFrame({0.0, 0})
    , currentSprite(nullptr)
{
    std::string storeName = package->getPackageName() + ":" + std::string(path);
    if(!exists(storeName))
    {
        data = new SpriteMapData();
        data->loadFromPackage(rend, package, path);
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

void SpriteMap::setAnimationSprite(std::string name)
{
    if(data->sprites.find(name) == data->sprites.end())
        return;
    
    currentSprite = &data->sprites.find(name)->second;

}

void SpriteMap::setSprite(std::string name)
{
    if(data->sprites.find(name) == data->sprites.end())
        return;
    
    currentSprite = &data->sprites.find(name)->second;
    currentFrame = {0.0, 0};
    currentAnimation = nullptr;
}

void SpriteMap::startAnimation(std::string animation)
{
    if(data->animations.find(animation) == data->animations.end())
        return;

    currentAnimation = &data->animations.find(animation)->second;
    currentFrame = {0.0, 0};
    setAnimationSprite(currentAnimation->sprites[currentFrame.frame]);
}

void SpriteMap::update(double deltaT)
{
    if(currentAnimation != nullptr)
    {
        currentFrame.elapsedTime += deltaT;
        if(currentFrame.elapsedTime > 1000.0/currentAnimation->FPS)
        {
            currentFrame.elapsedTime -= 1000.0/currentAnimation->FPS;
            currentFrame.frame++;
            if(currentFrame.frame > currentAnimation->sprites.size()-1)
            {
                currentFrame = {0.0, 0};
                currentAnimation = nullptr;
            }
            else
            {
                setAnimationSprite(currentAnimation->sprites[currentFrame.frame]);
            }
        }

    }
}

void SpriteMap::draw(SDL_Renderer* rend, SDL_Rect* bodyPos)
{
    if(currentSprite != nullptr)
        SDL_RenderCopyEx(rend, currentSprite->texture, &currentSprite->position, bodyPos, 0.0, NULL, SDL_FLIP_NONE);
}