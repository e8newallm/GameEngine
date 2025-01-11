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

void SpriteMap::setAnimationSprite(const std::string& name)
{
    if(data->sprites.find(name) == data->sprites.end())
        return;
    
    currentSprite = &data->sprites.find(name)->second;

}

void SpriteMap::setSprite(const std::string& name)
{
    if(data->sprites.find(name) == data->sprites.end())
        return;
    
    currentSprite = &data->sprites.find(name)->second;
    currentFrame = {0.0, 0};
    currentAnimation = nullptr;
}

void SpriteMap::startAnimation(const std::string& animation)
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

void SpriteMap::draw(World* world, SDL_Rect* bodyPos)
{
    if(currentSprite != nullptr)
    {
        SDL_FRect bPos;
        SDL_RectToFRect(bodyPos, &bPos);
        SDL_RenderTexture(world->getRend(), currentSprite->texture, &currentSprite->position, &bPos);
    }
}