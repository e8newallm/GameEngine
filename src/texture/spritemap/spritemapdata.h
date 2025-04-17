#ifndef SPRITEMAPDATA_H
#define SPRITEMAPDATA_H

#include <SDL3/SDL_gpu.h>
#include <map>
#include <vector>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/schema.h>

#include <SDL3/SDL.h>

#include "tools/packager/packager.h"

#include "texture.h"

namespace GameEng
{
    /**
     * \brief A structure for holding a sprite's data for use with a SpriteMapData.
     *
     */
    struct Sprite
    {
        GPUTexture* texture;
        std::string textureName;
        SDL_FRect position;
    };

    /**
     * \brief A structure for holding a spritemap animation data for use with a SpriteMapData.
     *
     */
    struct Animation
    {
        double FPS;
        std::vector<std::string> sprites;
    };

    /**
     * \brief A class for holding the data of a SpriteMap.
     *
     */
    class SpriteMapData
    {
        public:
            /**
             * \brief Construct an empty SpriteMapData object.
             * 
             */
            SpriteMapData();

            /**
             * \brief Load SpriteMapData from a file.
             * 
             * \param configLocation The file location of a config.
             */
            void loadFromFile(const char* configLocation);

            /**
             * \brief Load SpriteMapData from a package.
             * 
             * \param package The package the load the data from.
             * \param path The path within the package to load the config from.
             */
            void loadFromPackage(PackageManager* package, const char* path);

            /**
             * \brief Load SpriteMapData from a string.
             * 
             * \param spriteConfig The SpriteMapData config file.
             * \param source The source of the config file (Used for error reporting where the failing config file came from).
             */
            void loadFromString(const char* spriteConfig, const char* source = "string config");

            /**
             * \brief Save the current SpriteMapData as a config file.
             * 
             * \param spriteConfig The location to save the SpriteMapData to.
             */
            void save(const char* spriteConfig);

            /**
             * \brief Turn the current SpriteMapData into a binary blob for saving.
             *
             * \return std::string The binary blob to be saved.
             */
            std::string serialise();

            PackageManager* package;

            std::map<std::string, GPUTexture*> textures;
            std::map<std::string, Sprite> sprites;
            std::map<std::string, Animation> animations;
    };
}

#endif