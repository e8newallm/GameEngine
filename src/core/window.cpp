#include <SDL_render.h>
#include <iostream>

#include "window.h"
#include "gamestate.h"

Window::Window(std::string name, int width, int height, int flags)
{
    win = SDL_CreateWindow(name.c_str(),
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            width, height, flags);

    rend = SDL_CreateRenderer(win, -1,
                              SDL_RENDERER_ACCELERATED);

}

Window::Window(std::string name, int flags)
{
    win = SDL_CreateWindow(name.c_str(),
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            0, 0, SDL_WINDOW_FULLSCREEN | flags);
                                
    rend = SDL_CreateRenderer(win, -1,
                              SDL_RENDERER_ACCELERATED);
}

void Window::render(World& world)
{
    if(((1.0f / fps) - std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - lastRender).count() <= 0))
    {
        SDL_RenderClear(rend);
        world.draw(lastRender);
        SDL_RenderPresent(rend);

        timer newLastRender = std::chrono::high_resolution_clock::now();
        double actualFPS = 1.0f / std::chrono::duration<double>(newLastRender - lastRender).count();
        std::cout << "FPS: " << actualFPS << "\r\n";
        lastRender = newLastRender;
    }
}

Window::~Window()
{
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
}