#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <iostream>
#include <mutex>

#include "world.h"
#include "physicsobject.h"
#include "image.h"
#include "logging.h"
#include "gamestate.h"

double PPS = 0;

World::World(SDL_GPUDevice* gpu, View viewport) :
    viewport(viewport)
    , gpu(gpu)
    , phyRunning(false)
    , lastPhysics()
    , usageLock()
{

}

World::~World()
{
    for(const std::vector<Image*>& level : images)
    {
        for(Image* image : level)
        {
            delete image;
        }
    }

    for(PhysicsObject* obj : phyObjects)
    {
        delete obj;
    }
}

void World::draw(double deltaTime, SDL_Window* win)
{
    std::lock_guard<std::mutex> lock(usageLock);
    double percent = lastPhysics.getElapsed() / (1000.0f / pps);

    SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(getGPU());
    SDL_GPUTexture* swapchainTexture;
    if (!SDL_AcquireGPUSwapchainTexture(cmdbuf, win, &swapchainTexture, NULL, NULL))
    {
        Logger::error("SDL_AcquireGPUSwapchainTexture failed!");
        return;
    }
    if(swapchainTexture == NULL)
        return;

    SDL_GPUColorTargetInfo colorTargetInfo;
    SDL_zero(colorTargetInfo);
    colorTargetInfo.texture = swapchainTexture;
    colorTargetInfo.clear_color = (SDL_FColor){ 0.0f, 0.0f, 0.0f, 1.0f };
    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

    ShaderWorldData worldData {*getView().window()};
	SDL_PushGPUVertexUniformData(cmdbuf, 0, &worldData, sizeof(worldData));

    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, NULL);

    for(int i = UINT8_MAX; i > 128; i--)
    {
        for(Image* image : images[i])
        {
            image->draw(this, cmdbuf, renderPass);
        }
    }

    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        phyObjects[i]->draw(this, cmdbuf, renderPass, percent, deltaTime);
    }

    for(int i = 127; i >= 0; i--)
    {
        for(Image* image : images[i])
        {
            image->draw(this, cmdbuf, renderPass);
        }
    }

    SDL_EndGPURenderPass(renderPass);
    SDL_SubmitGPUCommandBuffer(cmdbuf);
}

void World::update()
{
    std::lock_guard<std::mutex> lock(usageLock);
    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        phyObjects[i]->update(pps, *this);
    }
}

void World::startPhysics()
{
    phyRunning = true;
}

void World::stopPhysics()
{
    phyRunning = false;
}

void World::runPhysics()
{
    if(lastPhysics.getElapsed() >= 900.0f / pps) //Check if physics loop is approaching the correct timing
    {
        while(lastPhysics.getElapsed() < (1000.0f / pps)); //Busy loop to get the timing correct
        PPS = 1000.0f / lastPhysics.getElapsed();
        lastPhysics.update();

        if(!GameState::gamePaused() && phyRunning) update();
    }
}

void World::addImage(Image* newImage)
{
    this->images[UINT8_MAX].push_back(newImage);
};

void World::addPhyObj(PhysicsObject* obj)
{
    phyObjects.push_back(obj);
}

View& World::getView()
{
    return viewport;
}

SDL_GPUDevice* World::getGPU()
{
    return gpu;
}