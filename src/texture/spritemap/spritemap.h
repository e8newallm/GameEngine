#ifndef SPRITEMAP_H
#define SPRITEMAP_H

#include <string>

#include "texture_base.h"
#include "spritemapdata.h"
#include "datastore.h"

#include "tools/packager/packager.h"

class SpriteMap : public Texture_base, public DataStore<SpriteMapData, SpriteMap>
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