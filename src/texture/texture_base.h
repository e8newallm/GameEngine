#ifndef TEXTURE_BASE_H
#define TEXTURE_BASE_H

#include <SDL3/SDL.h>

#include "world.h"

namespace GameEng
{
    /**
    * \brief Pure virtual base class for textures.
    *
    * A texture class to be used as the base for all derived texture classes.
    */
    class Texture_base
    {
        public:
            /**
             * \brief Pure virtual function for handling game logic.
             *
             * \param deltaT How much time has passed since the last update (in milliseconds).
             */
            virtual void update(double deltaT) = 0;

            /**
             * \brief Pure virtual function for handling object drawing.
             *
             * \param world The world that is being drawn to.
             * \param buffer The buffer that contains the shader data.
             * \param renderPass The render pass for the current draw.
             */
            virtual void draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass) = 0;

            /**
             * \brief Get the texture UV rectangle for use in shaders.
             *
             * \return SDL_FRect The UV rectangle to be used by the shader.
             */
            virtual SDL_FRect getUV() { return SDL_FRect{0.0f, 0.0f, 1.0f, 1.0f}; };
            virtual ~Texture_base() {};
    };
}
#endif