#ifndef SPRITEMAP_H
#define SPRITEMAP_H

#include <map>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/schema.h>
#include <string>

#include "texture.h"

struct Sprite
{
    SDL_Texture* texture;
    SDL_Rect position;
};

struct Animation
{
    double FPS;
    std::vector<std::string> sprites;
};

struct CurrentFrame
{
    double elapsedTime = 0.0;
    Uint32 frame = 0;
};

class SpriteMap : public Texture
{
    public:
        SpriteMap(SDL_Renderer* rend, const char* spriteConfig);
        TexRequest getTexture() override;
        void setSprite(std::string name);
        void setAnimationSprite(std::string name);
        void startAnimation(std::string animation);
        void update(double deltaT) override;
        bool animationRunning() {return currentAnimation != nullptr; };

    private:
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, Sprite> sprites;
        std::map<std::string, Animation> animations;
        Animation* currentAnimation;
        CurrentFrame currentFrame;
        Sprite* currentSprite;
        static rapidjson::SchemaValidator* validator;
};

#endif