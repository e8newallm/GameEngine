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
        SpriteMap(SDL_GPUDevice* gpu, const char* spriteConfig);
        SpriteMap(SDL_GPUDevice* gpu, PackageManager* package, const char* path);
        explicit SpriteMap(SpriteMapData* spriteData);

        virtual SDL_FRect getUV() override;
        virtual void update(double deltaT) override;
        virtual void draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass) override;

        void setSprite(const std::string& name);
        void setAnimationSprite(const std::string& name);
        void startAnimation(const std::string& animation);
        bool animationRunning() {return currentAnimation != nullptr; };

    private:
        Animation* currentAnimation;
        CurrentFrame currentFrame;
        Sprite* currentSprite;
        SpriteMapData* data;
};

#endif