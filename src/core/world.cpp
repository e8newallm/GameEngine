#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <iostream>
#include <mutex>

#include "world.h"
#include "physicsobject.h"
#include "image.h"
#include "logging.h"
#include "gamestate.h"
#include "graphics.h"

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
    for(Object* obj : objects)
    {
        delete obj;
    }
}

void World::update(double deltaTime)
{
    std::lock_guard<std::mutex> lock(usageLock);

    setPhyInterpolation(lastPhysics.getElapsed() / (1000.0f / pps));

    if(updateFunc != nullptr)
        (*updateFunc)(deltaTime, *this);

    for(Object* obj : objects)
        obj->update(deltaTime, *this);
}

void World::draw(double deltaTime, SDL_Window* win)
{
    std::lock_guard<std::mutex> lock(usageLock);

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
    colorTargetInfo.clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

    std::vector<ShaderObjData> data{};
    std::vector<Uint32> indexes{};
    Uint32 dataSize = 0;

    for(uint64_t i = 0; i < objects.size(); i++)
    {
        ShaderObjData newData = objects[i]->predraw();
        indexes.push_back(dataSize);
        dataSize += newData.size;
        data.push_back(newData);
    }

    //FUTURE FUNCTION

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer* objectDataBuffer;

    SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo;
    SDL_zero(transferBufferCreateInfo);
    transferBufferCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transferBufferCreateInfo.size = dataSize + (Uint32)indexes.size() * (Uint32)sizeof(Uint32);
    transferBuffer = SDL_CreateGPUTransferBuffer(gpu, &transferBufferCreateInfo);

    SDL_GPUBufferCreateInfo bufferCreateInfo;
    SDL_zero(bufferCreateInfo);
    bufferCreateInfo.usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ;
    bufferCreateInfo.size = dataSize + (Uint32)indexes.size() * (Uint32)sizeof(Uint32);
    objectDataBuffer = SDL_CreateGPUBuffer(gpu, &bufferCreateInfo);

    Uint32* dataPtr = static_cast<Uint32*>(SDL_MapGPUTransferBuffer(gpu,transferBuffer,true));

    int address = 0;
    for(Uint32 index : indexes)
    {
        dataPtr[address++] = index + indexes.size() * 4;
    }

    for(ShaderObjData objData : data)
    {
        SDL_memcpy(&dataPtr[address], objData.data, objData.size);
        address += objData.size/4;
    }

    SDL_UnmapGPUTransferBuffer(gpu, transferBuffer);

    // Upload instance data
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdbuf);

    SDL_GPUTransferBufferLocation transferBufferLocation;
    SDL_zero(transferBufferLocation);
    transferBufferLocation.transfer_buffer = transferBuffer;
    transferBufferLocation.offset = 0;

    SDL_GPUBufferRegion bufferRegion;
    SDL_zero(bufferRegion);
    bufferRegion.buffer = objectDataBuffer;
    bufferRegion.offset = 0;
    bufferRegion.size = dataSize + (Uint32)indexes.size() * 4;

    SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &bufferRegion,true);

    SDL_EndGPUCopyPass(copyPass);

    // END OF FUTURE FUNCTION

    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, NULL);

    ShaderWorldData worldData {*getView().window()};
	SDL_PushGPUVertexUniformData(cmdbuf, 1, &worldData, sizeof(ShaderWorldData));

    int drawID = 0;

    for(uint64_t i = 0; i < objects.size(); i++)
    {
        SDL_PushGPUVertexUniformData(cmdbuf, 0, &drawID, sizeof(Uint32));
        drawID++;
        objects[i]->draw(this, objectDataBuffer, renderPass, deltaTime);
    }

    SDL_EndGPURenderPass(renderPass);
    SDL_SubmitGPUCommandBuffer(cmdbuf);

    for(ShaderObjData test : data)
        free(test.data);
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

        if(!GameState::gamePaused() && phyRunning)
        {
            std::lock_guard<std::mutex> lock(usageLock);
            for(uint64_t i = 0; i < objects.size(); i++)
            {
                objects[i]->runPhysics(pps, *this);
            }
        }
    }
}

void World::registerUpdate(GEUpdateFunc func)
{
    updateFunc = func;
}

void World::addObj(Object* obj)
{
    objects.push_back(obj);
}

View& World::getView()
{
    return viewport;
}

SDL_GPUDevice* World::getGPU()
{
    return gpu;
}