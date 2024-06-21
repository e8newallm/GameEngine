#ifndef SPRITEMAP_H
#define SPRITEMAP_H

#include <map>
#include <vector>
#include <string>

#include "texture.h"
#include "spritemapdata.h"
#include "tools/packager/packager.h"

class SpriteMap : public Texture
{
    public:
        SpriteMap(SDL_Renderer* rend, const char* spriteConfig);
        SpriteMap(SDL_Renderer* rend, PackageManager* package, const char* path);
        SpriteMap(SDL_Renderer* rend, SpriteMapData* spriteData);

        TexRequest getTexture() override;
        void setSprite(std::string name);
        void setAnimationSprite(std::string name);
        void startAnimation(std::string animation);
        void update(double deltaT) override;
        bool animationRunning() {return currentAnimation != nullptr; };

    private:
        Animation* currentAnimation;
        CurrentFrame currentFrame;
        Sprite* currentSprite;
        SpriteMapData* data;
};

#endif