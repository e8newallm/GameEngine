#include <SDL3/SDL.h>
#include <iostream>

#include "window.h"
#include "gamestate.h"

double FPS = 0;

Window::Window(const std::string& name, int width, int height, int flags)
{
    win = SDL_CreateWindow(name.c_str(),
                            width, height,  SDL_WINDOW_VULKAN | flags);

    rend = SDL_CreateRenderer(win, NULL);

}

Window::Window(const std::string& name, int flags)
{
    win = SDL_CreateWindow(name.c_str(),
                            0, 0, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_VULKAN | flags);

    rend = SDL_CreateRenderer(win, NULL);
}

void Window::render(World& world)
{
    if(lastRender.getElapsed() >= (800.0f / fps)) //Check if render loop is approaching the correct timing
    {
        while(lastRender.getElapsed() < (1000.0f / fps)); //Busy loop to get the timing correct

        double deltaTime = lastRender.getElapsed();
        FPS = 1000.0f / lastRender.getElapsed();
        lastRender.update();

        SDL_RenderClear(rend);
        world.draw(deltaTime);
        SDL_RenderPresent(rend);
    }
}

Window::~Window()
{
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
}