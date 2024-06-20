#include "spritemap.h"

SpriteMap::SpriteMap(SDL_Renderer* rend, const char* spriteConfig) :
    Texture(rend, nullptr)
    , currentAnimation(nullptr)
    , currentFrame({0.0, 0})
    , currentSprite(nullptr)
    , data(new SpriteMapData())
{
    data->loadFromFile(rend, spriteConfig);
}

SpriteMap::SpriteMap(SDL_Renderer* rend, SpriteMapData* spriteData) :
    Texture(rend, nullptr)
    , currentAnimation(nullptr)
    , currentFrame({0.0, 0})
    , currentSprite(nullptr)
    , data(spriteData)
{
}

TexRequest SpriteMap::getTexture()
{
    if(currentSprite == nullptr)
        return {nullptr, {0, 0, 0, 0}};

    return {currentSprite->texture, currentSprite->position};
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