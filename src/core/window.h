#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

#include "world.h"

class Window
{
    public:
        Window(std::string name, int width, int height, int flags);
        Window(std::string name, int flags);
        ~Window();

        SDL_Renderer* getRend() const { return rend; };
        void render(World& world);
    private:
        SDL_Window* win;
        SDL_Renderer* rend;
};

#endif