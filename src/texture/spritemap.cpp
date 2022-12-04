#include <iostream>
#include <rapidjson/document.h>
#include <fstream>
#include <sstream>

#include "spritemap.h"

SpriteMap::SpriteMap(SDL_Renderer* rend, const char* spriteConfig) :
    Texture(rend, nullptr)
    , sprites()
    , currentAnimation(nullptr)
    , currentFrame({0.0, 0})
    , currentSprite(nullptr)
{
    std::ifstream file(spriteConfig);
    std::ostringstream ss;
    ss << file.rdbuf();
    rapidjson::Document config;
    config.Parse(ss.str().c_str());

    if(config["Textures"].IsArray())
    {
        for(rapidjson::Value& value : config["Textures"].GetArray())
        {
            std::pair<const char*, SDL_Texture*> newTexture = std::make_pair(value.GetString(),
                                        SDL_CreateTextureFromSurface(rend, IMG_Load(value.GetString())));
            textures.insert(newTexture);
        }
    }
    else
    {
        std::pair<const char*, SDL_Texture*> newTexture = std::make_pair(config["Textures"].GetString(),
                                            SDL_CreateTextureFromSurface(rend, IMG_Load(config["Textures"].GetString())));
        textures.insert(newTexture);
    }
    
    for(rapidjson::Value& value : config["Sprites"].GetArray())
    {
        Sprite newEntry;
        rapidjson::GenericObject<false, rapidjson::Value> sprite = value.GetObject();
        newEntry.position = { sprite.FindMember("x")->value.GetInt()
                             , sprite.FindMember("y")->value.GetInt()
                             , sprite.FindMember("height")->value.GetInt()
                             , sprite.FindMember("width")->value.GetInt() };
        newEntry.texture = textures.find(sprite.FindMember("texture")->value.GetString())->second;
        sprites.insert(std::make_pair(sprite.FindMember("name")->value.GetString(), newEntry));
    }

    for(rapidjson::Value& value : config["Animations"].GetArray())
    {
        rapidjson::GenericObject<false, rapidjson::Value> animation = value.GetObject();
        Animation newAni;
        newAni.FPS = animation.FindMember("FPS")->value.GetDouble();
        for(rapidjson::Value& i : animation.FindMember("frames")->value.GetArray())
        {
            newAni.sprites.push_back((char*)i.GetString());
        }
        animations.insert(std::make_pair(animation.FindMember("name")->value.GetString(), newAni));
    }
}

TexRequest SpriteMap::getTexture()
{
    if(currentSprite == nullptr)
        return {nullptr, {0, 0, 0, 0}};

    return {currentSprite->texture, currentSprite->position};
}
void SpriteMap::setAnimationSprite(std::string name)
{
    if(sprites.find(name) == sprites.end())
        return;
    
    currentSprite = &sprites.find(name)->second;

}

void SpriteMap::setSprite(std::string name)
{
    if(sprites.find(name) == sprites.end())
        return;
    
    currentSprite = &sprites.find(name)->second;
    currentFrame = {0.0, 0};
    currentAnimation = nullptr;
}

void SpriteMap::startAnimation(std::string animation)
{
    if(animations.find(animation) == animations.end())
        return;

    currentAnimation = &animations.find(animation)->second;
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