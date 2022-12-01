#include <iostream>
#include <rapidjson/document.h>

#include "spritemap.h"

SpriteMap::SpriteMap(SDL_Renderer* rend, const char* texturePath, const char* spriteConfig) :
    Texture(rend, texturePath)
    , sprites()
    , currentSprite()
{
    rapidjson::Document config;
    config.Parse(spriteConfig);
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
        rapidjson::GenericObject<false, rapidjson::Value> Sprite = value.GetObject();
    }
}

TexRequest SpriteMap::getTexture()
{
    return {currentSprite->second.texture, currentSprite->second.position};
}
void SpriteMap::setSprite(char* name)
{
    currentSprite = sprites.find(name);
}