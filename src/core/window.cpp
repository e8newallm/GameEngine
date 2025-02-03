#include <SDL3/SDL.h>

#include "window.h"
#include "gamestate.h"

double FPS = 0;

Window::Window(const std::string& name, int width, int height, int flags)
{
    win = SDL_CreateWindow(name.c_str(),
                            width, height,  SDL_WINDOW_VULKAN | flags);
    if (win == NULL)
	{
		SDL_Log("CreateWindow failed: %s", SDL_GetError());
	}
    gpu = SDL_CreateGPUDevice(
		SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
		false,
		NULL);
    if (gpu == NULL)
	{
		SDL_Log("GPUCreateDevice failed");
	}

    if (!SDL_ClaimWindowForGPUDevice(gpu, win))
	{
		SDL_Log("GPUClaimWindow failed");
	}

	SDL_SetGPUSwapchainParameters(
		gpu,
		win,
		SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
		SDL_GPU_PRESENTMODE_IMMEDIATE
	);

    reso = {width, height};

}

Window::Window(const std::string& name, int flags) : Window(name, 0, 0, SDL_WINDOW_FULLSCREEN | flags)
{
}

const SDL_Point* Window::getResolution() const
{
    return &reso;
}

void Window::render(World& world)
{
    if(lastRender.getElapsed() >= (800.0f / fps)) //Check if render loop is approaching the correct timing
    {
        while(lastRender.getElapsed() < (1000.0f / fps)); //Busy loop to get the timing correct

        double deltaTime = lastRender.getElapsed();
        FPS = 1000.0f / lastRender.getElapsed();
        lastRender.update();

        world.draw(deltaTime, getWin());
    }
}

Window::~Window()
{
    SDL_ReleaseWindowFromGPUDevice(gpu, win);
    SDL_DestroyWindow(win);
    SDL_DestroyGPUDevice(gpu);
}