#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <string>

#include "world.h"
#include "timer.h"

namespace GameEng
{
    /**
     * \brief A class for handling window creation/interation/destruction.
     * 
     */
    class Window
    {
        public:
            /**
             * \brief Construct a new window using Vulkan.
             *
             * \param name The name of the window.
             * \param width The width of the window.
             * \param height The height of the window.
             * \param flags Any SDL_WindowFlags required for the window.
             * \param newGPU The GPU device to be used for this window.
             */
            Window(const std::string& name, int width, int height, int flags, SDL_GPUDevice* newGPU = nullptr);

            /**
             * \brief Construct a new full screen window using Vulkan.
             *
             * \param name The name of the window.
             * \param flags Any SDL_WindowFlags required for the window.
             * \param newGPU The GPU device to be used for this window.
             */
            Window(const std::string& name, int flags, SDL_GPUDevice* newGPU = nullptr);
            ~Window();

            /**
             * \brief Gets the GPU device of the window.
             * 
             * \return SDL_GPUDevice* The GPU device of the window.
             */
            SDL_GPUDevice* getGPU() const { return gpu; };

            /**
             * \brief Get the SDL window object.
             * 
             * \return SDL_Window* The SDL window.
             */
            SDL_Window* getWin() const { return win; };

            /**
             * \brief Get the resolution of the window.
             * 
             * \return const SDL_Point* The resolution of the window.
             */
            const SDL_Point* getResolution() const { return &reso; };

            /**
             * \brief Renders a frame for the window using a given world.
             *
             * \param world The world to be rendered.
             */
            void render(World& world);

        private:
            Window() = delete;
            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;

            SDL_Window* win;
            SDL_GPUDevice* gpu;
            SDL_Point reso;

            double fps = 165.0f;
            Timer renderTimer;
    };
}

#endif