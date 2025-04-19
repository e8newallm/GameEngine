#ifndef SPRITEMAP_H
#define SPRITEMAP_H

#include <string>

#include "texture_base.h"
#include "spritemapdata.h"
#include "datastore.h"

#include "tools/packager/packager.h"

namespace GameEng
{
    /**
     * \brief A structure for keeping track of a SpriteMap's current sprite in a running animation.
     *
     */
     struct CurrentFrame
     {
         double elapsedTime = 0.0;
         Uint32 frame = 0;
     };


    /**
     * \brief A Texture_base derived class for drawing spritesheets.
     *
     */
    class SpriteMap : public Texture_base, public DataStore<SpriteMapData, SpriteMap>
    {
        public:
            /**
             * \brief Construct a new SpriteMap from loading a config file.
             *
             * \param configLocation The config location to be loaded.
             */
            explicit SpriteMap(const char* configLocation);

            /**
             * \brief Construct a new SpriteMap from a package.
             *
             * \param package The package that the SpriteMap is loaded from.
             * \param path The path inside the package that the SpriteMap is located at.
             */
            SpriteMap(const Packager::PackageManager* package, const char* path);

            /**
             * \brief Construct a new SpriteMap from an existing SpriteMapData.
             *
             * \param spriteData The existing SpriteMapData.
             */
            explicit SpriteMap(SpriteMapData* spriteData);

            /**
             * \brief Get the current sprite's UV rectangle for use in shaders.
             *
             * \return SDL_FRect The current sprite's UV rectangle to be used by the shader.
             */
            virtual SDL_FRect getUV() override;

            /**
             * \brief Handles any animating of the sprite.
             *
             * \param deltaT How much time has passed since the last update (in milliseconds).
             */
            virtual void update(double deltaT) override;

            /**
             * \brief Draws the current sprite to the world.
             *
             * \param world The world the sprite will be drawn to.
             * \param buffer The buffer that contains the shader data.
             * \param renderPass The render pass for the current draw.
             */
            virtual void draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass) override;

            /**
             * \brief Set the current sprite to the named sprite and cancel any animation currently running.
             *
             * \param name The name of the specific frame to set to.
             */
            void setSprite(const std::string& name);

            /**
             * \brief Set the current sprite to a named sprite without cancelling any current animation.
             * Primarily used for moving to the next sprite in an animation during the update function.
             *
             * \param name
             */
            void setAnimationSprite(const std::string& name);

            /**
             * \brief Starts an animation running.
             * 
             * \param animation The animation to run.
             */
            void startAnimation(const std::string& animation);

            /**
             * \brief Check if an animation is currently running.
             * 
             * \return bool Is an animation currently running?
             */
            bool animationRunning() const {return currentAnimation != nullptr; };

        private:
            const Animation* currentAnimation;
            CurrentFrame currentFrame;
            const Sprite* currentSprite;
            std::shared_ptr<SpriteMapData> data;
    };
}

#endif