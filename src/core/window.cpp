#include "window.h"

Window::Window(std::string name, int width, int height, int flags)
{
    win = SDL_CreateWindow(name.c_str(),
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            width, height, flags);

    rend = SDL_CreateRenderer(win, -1,
                              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

}

Window::Window(std::string name, int flags)
{
    win = SDL_CreateWindow(name.c_str(),
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            0, 0, SDL_WINDOW_FULLSCREEN | flags);
                                
    rend = SDL_CreateRenderer(win, -1,
                              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Window::render(World& world)
{
    SDL_RenderClear(rend);
    world.draw();
    SDL_RenderPresent(rend);
}

Window::~Window()
{
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
}