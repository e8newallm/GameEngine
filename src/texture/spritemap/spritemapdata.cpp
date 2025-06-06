#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/schema.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "SpriteMapSchema.h"
#include "geerror.h"
#include "spritemapdata.h"
#include "texture.h"
#include "tools/packager/packager.h"

namespace GameEng
{
SpriteMapData::SpriteMapData() : package()
{
}

void SpriteMapData::loadFromFile(const char* configLocation)
{
    std::ifstream file(configLocation);

    if(!file.is_open())
    {
        throw GameEngineException(GEError::FILE_NOT_FOUND,
                                  std::string("Could not open Spritemap JSON \"") + configLocation + "\"");
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    loadFromString(ss.str().c_str(), configLocation);
}

void SpriteMapData::loadFromPackage(const Packager::PackageManager* package, const char* path)
{
    this->package = package;
    std::vector<uint8_t> data = package->getFile(path);
    loadFromString(std::string(data.begin(), data.end()).c_str(), (package->getPackageName() + ":" + path).c_str());
}

void SpriteMapData::loadFromString(const char* spriteConfig, const char* source)
{
    rapidjson::Document schema;
    schema.Parse(SpriteMapSchema);
    rapidjson::SchemaDocument schemaDoc(schema);
    rapidjson::SchemaValidator validator(schemaDoc);

    rapidjson::Document config;
    config.Parse(spriteConfig);

    if(config.HasParseError())
    {
        throw GameEngineException(GEError::INVALID_FILE_FORMAT, std::string("\"") + source + "\" is invalid JSON");
    }

    if(!config.Accept(validator))
    {
        rapidjson::StringBuffer sb;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        throw GameEngineException(GEError::INVALID_FILE_FORMAT,
                                  std::string("\"") + source + "\" has failed to pass SpriteMap schema");
    }

    for(rapidjson::Value& value : config["Textures"].GetArray())
    {
        if(!Texture::exists(value.GetString()))
        {
            const SDL_Surface* surface;
            if(package != nullptr)
            {
                std::vector<uint8_t> data = package->getFile(value.GetString());
                SDL_IOStream* dataBuffer = SDL_IOFromMem(data.data(), data.size());
                surface = IMG_Load_IO(dataBuffer, true);
            }
            else
            {
                surface = IMG_Load(value.GetString());
            }
            if(surface == nullptr)
            {
                throw GameEngineException(GEError::FILE_NOT_FOUND, std::string("\"") + source +
                                                                       "\" could not load texture file \"" +
                                                                       value.GetString() + "\"");
            }
        }
        textures.insert({value.GetString(), Texture::get(value.GetString())});
    }

    for(rapidjson::Value& value : config["Sprites"].GetArray())
    {
        Sprite newEntry;
        rapidjson::GenericObject<false, rapidjson::Value> sprite = value.GetObject();
        if(sprites.contains(sprite.FindMember("name")->value.GetString()))
        {
            throw GameEngineException(GEError::INVALID_FILE_FORMAT,
                                      std::string("\"") + source + "\" has two sprites named (" +
                                          sprite.FindMember("name")->value.GetString() + ")");
        }

        if(!textures.contains(sprite.FindMember("texture")->value.GetString()))
        {
            throw GameEngineException(GEError::INVALID_FILE_FORMAT,
                                      std::string("\"") + source + "\" has a sprite (" +
                                          sprite.FindMember("name")->value.GetString() +
                                          ") referencing a texture not named in the JSON");
        }

        newEntry.position = {.x = sprite.FindMember("x")->value.GetFloat(),
                             .y = sprite.FindMember("y")->value.GetFloat(),
                             .w = sprite.FindMember("height")->value.GetFloat(),
                             .h = sprite.FindMember("width")->value.GetFloat()};
        newEntry.texture = Texture::get(sprite.FindMember("texture")->value.GetString());
        newEntry.textureName = sprite.FindMember("texture")->value.GetString();
        sprites.insert(std::make_pair(sprite.FindMember("name")->value.GetString(), newEntry));
    }

    if(config.HasMember("Animations"))
    {
        for(rapidjson::Value& value : config["Animations"].GetArray())
        {
            Animation newAni;
            rapidjson::GenericObject<false, rapidjson::Value> animation = value.GetObject();
            if(animations.contains(animation.FindMember("name")->value.GetString()))
            {
                throw GameEngineException(GEError::INVALID_FILE_FORMAT,
                                          std::string("\"") + source + "\" has two animations named (" +
                                              animation.FindMember("name")->value.GetString() + ")");
            }

            newAni.FPS = animation.FindMember("FPS")->value.GetDouble();
            for(rapidjson::Value& i : animation.FindMember("frames")->value.GetArray())
            {
                if(!sprites.contains(i.GetString()))
                {
                    throw GameEngineException(GEError::INVALID_FILE_FORMAT,
                                              std::string("\"") + source + "\" has a animation (" +
                                                  animation.FindMember("name")->value.GetString() +
                                                  ") referencing a sprite not named in the JSON");
                }

                newAni.sprites.emplace_back(i.GetString());
            }
            animations.insert(std::make_pair(animation.FindMember("name")->value.GetString(), newAni));
        }
    }
}

void SpriteMapData::save(const char* spriteConfig)
{
    std::string data = serialise();

    std::ofstream output;
    output.open(spriteConfig, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    output << data;
    output.close();
}

std::string SpriteMapData::serialise()
{
    rapidjson::Document schema;
    schema.Parse(SpriteMapSchema);
    rapidjson::SchemaDocument schemaDoc(schema);
    rapidjson::SchemaValidator validator(schemaDoc);

    rapidjson::Document config;
    rapidjson::Document::AllocatorType& allocator = config.GetAllocator();
    config.SetObject();

    if(textures.empty())
    {
        throw GameEngineException(GEError::INVALID_FILE_FORMAT, "Serialise: Spritemap JSON has no textures defined!");
    }

    rapidjson::Value texArray(rapidjson::kArrayType);
    for(std::pair<const std::string, std::shared_ptr<GPUTexture>>& texture : textures)
    {
        texArray.PushBack(rapidjson::Value(texture.first.c_str(), allocator), allocator);
    }
    config.AddMember("Textures", texArray, allocator);

    rapidjson::Value sprArray(rapidjson::kArrayType);
    for(auto& sprite : sprites)
    {
        rapidjson::Value value;
        value.SetObject();
        value.AddMember("name", rapidjson::Value().SetString(sprite.first.c_str(), allocator), allocator);
        value.AddMember("texture", rapidjson::Value().SetString(sprite.second.textureName.c_str(), allocator),
                        allocator);
        value.AddMember("x", rapidjson::Value().SetUint64(static_cast<uint64_t>(sprite.second.position.x)), allocator);
        value.AddMember("y", rapidjson::Value().SetUint64(static_cast<uint64_t>(sprite.second.position.y)), allocator);
        value.AddMember("width", rapidjson::Value().SetUint64(static_cast<uint64_t>(sprite.second.position.w)),
                        allocator);
        value.AddMember("height", rapidjson::Value().SetUint64(static_cast<uint64_t>(sprite.second.position.h)),
                        allocator);
        sprArray.PushBack(value, allocator);
    }
    config.AddMember("Sprites", sprArray, allocator);

    if(!animations.empty())
    {
        rapidjson::Value aniArray(rapidjson::kArrayType);
        for(auto& animation : animations)
        {
            rapidjson::Value value;
            value.SetObject();
            value.AddMember("name", rapidjson::Value().SetString(animation.first.c_str(), allocator), allocator);
            value.AddMember("FPS", rapidjson::Value().SetFloat(static_cast<float>(animation.second.FPS)), allocator);
            rapidjson::Value frameArray(rapidjson::kArrayType);
            for(const std::string& sprite : animation.second.sprites)
            {
                frameArray.PushBack(rapidjson::Value().SetString(sprite.c_str(), allocator), allocator);
            }
            value.AddMember("frames", frameArray, allocator);
            aniArray.PushBack(value, allocator);
        }
        config.AddMember("Animations", aniArray, allocator);
    }

    if(!config.Accept(validator))
    {
        rapidjson::StringBuffer sb;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        throw GameEngineException(GEError::INVALID_FILE_FORMAT,
                                  "Serialise: Spritemap JSON has failed to pass SpriteMap schema");
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    config.Accept(writer);

    return buffer.GetString();
}
} // namespace GameEng