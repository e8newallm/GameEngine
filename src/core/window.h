#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

#include "world.h"
#include "timer.h"

class Window
{
    public:
        Window() = delete;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(const std::string& name, int width, int height, int flags);
        Window(const std::string& name, int flags);
        ~Window();

        SDL_Renderer* getRend() const { return rend; };
        void render(World& world);
        
    private:
        SDL_Window* win;
        SDL_Renderer* rend;
        double fps = 165.0f;
        Timer<> lastRender;
};

#endif