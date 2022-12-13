#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/schema.h>
#include <rapidjson/stringbuffer.h>

#include "spritemap.h"
#include "schema.h"

SpriteMap::SpriteMap(SDL_Renderer* rend, const char* spriteConfig) :
    Texture(rend, nullptr)
    , sprites()
    , currentAnimation(nullptr)
    , currentFrame({0.0, 0})
    , currentSprite(nullptr)
{
    rapidjson::Document schema;
    schema.Parse(SpriteMapSchema);
    rapidjson::SchemaDocument schemaDoc(schema);
    rapidjson::SchemaValidator validator(schemaDoc);

    std::ifstream file(spriteConfig);

    if(!file.is_open())
    {
        throw std::runtime_error(std::string("Could not open Spritemap JSON \"") + spriteConfig + "\"");
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    rapidjson::Document config;
    config.Parse(ss.str().c_str());

    if(config.HasParseError())
    {
        throw std::runtime_error(std::string("\"") + spriteConfig + "\" is invalid JSON");
    }

    if(!config.Accept(validator))
    {
        rapidjson::StringBuffer sb;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        throw std::runtime_error(std::string("\"") + spriteConfig + "\" has failed to pass SpriteMap schema");
    }

    if(config["Textures"].IsArray())
    {
        for(rapidjson::Value& value : config["Textures"].GetArray())
        {
            SDL_Surface* surface = IMG_Load(value.GetString());
            if(surface == nullptr)
            {
                throw std::runtime_error(std::string("\"") + spriteConfig + "\" could not load texture file \"" + value.GetString() + "\"");
            }

            std::pair<const char*, SDL_Texture*> newTexture = std::make_pair(value.GetString(),
                                        SDL_CreateTextureFromSurface(rend, surface));
            textures.insert(newTexture);
        }
    }
    else
    {
        SDL_Surface* surface = IMG_Load(config["Textures"].GetString());
        if(surface == nullptr)
        {
            throw std::runtime_error(std::string("\"") + spriteConfig + "\" could not load texture file \"" + config["Textures"].GetString() + "\"");
        }
        std::pair<const char*, SDL_Texture*> newTexture = std::make_pair(config["Textures"].GetString(),
                                            SDL_CreateTextureFromSurface(rend, surface));
        textures.insert(newTexture);
    }
    
    for(rapidjson::Value& value : config["Sprites"].GetArray())
    {
        Sprite newEntry;
        rapidjson::GenericObject<false, rapidjson::Value> sprite = value.GetObject();
        if(sprites.find(sprite.FindMember("name")->value.GetString()) != sprites.end())
        {
            throw std::runtime_error(std::string("\"") + spriteConfig + "\" has two sprites named (" + sprite.FindMember("name")->value.GetString() + ")");
        }
        if(textures.find(sprite.FindMember("texture")->value.GetString()) == textures.end())
        {
            throw std::runtime_error(std::string("\"") + spriteConfig + "\" has a sprite (" + 
                                    sprite.FindMember("name")->value.GetString() + ") referencing a texture not named in the JSON");
        }

        newEntry.position = { sprite.FindMember("x")->value.GetInt()
                             , sprite.FindMember("y")->value.GetInt()
                             , sprite.FindMember("height")->value.GetInt()
                             , sprite.FindMember("width")->value.GetInt() };
        newEntry.texture = textures.find(sprite.FindMember("texture")->value.GetString())->second;
        sprites.insert(std::make_pair(sprite.FindMember("name")->value.GetString(), newEntry));
    }

    for(rapidjson::Value& value : config["Animations"].GetArray())
    {
        Animation newAni;
        rapidjson::GenericObject<false, rapidjson::Value> animation = value.GetObject();
        if(animations.find(animation.FindMember("name")->value.GetString()) != animations.end())
        {
            throw std::runtime_error(std::string("\"") + spriteConfig + "\" has two animations named (" + animation.FindMember("name")->value.GetString() + ")");
        }

        newAni.FPS = animation.FindMember("FPS")->value.GetDouble();
        for(rapidjson::Value& i : animation.FindMember("frames")->value.GetArray())
        {
            if(sprites.find(i.GetString()) == sprites.end())
            {
                throw std::runtime_error(std::string("\"") + spriteConfig + "\" has a animation (" + 
                                    animation.FindMember("name")->value.GetString() + ") referencing a sprite not named in the JSON");
            }

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