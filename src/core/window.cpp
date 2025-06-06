#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

#include "gamestate.h"
#include "window.h"

namespace GameEng
{
Window::Window(const std::string& name, int width, int height, int flags, SDL_GPUDevice* newGPU) : renderTimer(fps + 1)
{
    win = SDL_CreateWindow(name.c_str(), width, height, SDL_WINDOW_VULKAN | flags);
    if(win == nullptr)
    {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
    }

    gpu = newGPU;

    if(gpu == nullptr)
    {
        SDL_Log("GPUCreateDevice failed");
    }

    if(!SDL_ClaimWindowForGPUDevice(gpu, win))
    {
        SDL_Log("GPUClaimWindow failed");
    }

    SDL_SetGPUSwapchainParameters(gpu, win, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_IMMEDIATE);

    reso = {.x = width, .y = height};
}

Window::Window(const std::string& name, int flags, SDL_GPUDevice* newGPU)
    : Window(name, 0, 0, SDL_WINDOW_FULLSCREEN | flags, newGPU)
{
}

bool Window::render(World& world)
{
    if(renderTimer.trigger())
    {
        double deltaTime = renderTimer.getElapsed();
        renderTimer.update();
        world.update(deltaTime);
        world.draw(getWin());
        return true;
    }
    return false;
}

Window::~Window()
{
    SDL_ReleaseWindowFromGPUDevice(gpu, win);
    SDL_DestroyWindow(win);
}
} // namespace GameEng