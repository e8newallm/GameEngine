#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <mutex>

#include "gamestate.h"
#include "logging.h"
#include "object.h"
#include "physicsobject.h"
#include "world.h"

namespace GameEng
{
World::World(SDL_GPUDevice* gpu, View viewport) : viewport(viewport), gpu(gpu), phyRunning(false), physicsTimer(pps)
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
    static const float milliseconds = 1000.0F;
    std::lock_guard<std::mutex> lock(usageLock);

    setPhyInterpolation(physicsTimer.getElapsed() / (milliseconds / pps));

    if(updateFunc != nullptr)
    {
        (*updateFunc)(deltaTime, *this);
    }

    for(Object* obj : objects)
    {
        obj->update(deltaTime, *this);
    }
}

void World::draw(SDL_Window* win)
{
    std::lock_guard<std::mutex> lock(usageLock);

    SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(getGPU());
    SDL_GPUTexture* swapchainTexture;
    if(!SDL_AcquireGPUSwapchainTexture(cmdbuf, win, &swapchainTexture, nullptr, nullptr))
    {
        Logger::error("SDL_AcquireGPUSwapchainTexture failed!");
        return;
    }
    if(swapchainTexture == nullptr)
    {
        return;
    }

    SDL_GPUColorTargetInfo colorTargetInfo;
    SDL_zero(colorTargetInfo);
    colorTargetInfo.texture = swapchainTexture;
    colorTargetInfo.clear_color = {.r = 0.0F, .g = 0.0F, .b = 0.0F, .a = 1.0F};
    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

    std::vector<std::vector<std::byte>> data{};
    std::vector<Uint32> indexes{};
    Uint32 dataSize = 0;

    for(Object* object : objects)
    {
        std::vector<std::byte> newData = object->predraw();
        indexes.push_back(dataSize);
        dataSize += newData.size();
        data.push_back(newData);
    }

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

    Uint32* dataPtr = static_cast<Uint32*>(SDL_MapGPUTransferBuffer(gpu, transferBuffer, true));

    int address = 0;
    for(Uint32 index : indexes)
    {
        dataPtr[address++] = index + indexes.size() * 4;
    }

    for(std::vector<std::byte> objData : data)
    {
        SDL_memcpy(&dataPtr[address], objData.data(), objData.size());
        address += static_cast<int>(objData.size() / 4);
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

    SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &bufferRegion, true);
    SDL_EndGPUCopyPass(copyPass);

    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, nullptr);

    ShaderWorldData worldData{*getView().window()};
    SDL_PushGPUVertexUniformData(cmdbuf, 1, &worldData, sizeof(ShaderWorldData));

    int drawID = 0;
    for(Object* object : objects)
    {
        SDL_PushGPUVertexUniformData(cmdbuf, 0, &drawID, sizeof(Uint32));
        drawID++;
        if(SDL_HasRectIntersection(object->getBody(), getView().window()))
        {
            object->draw(this, objectDataBuffer, renderPass);
        }
    }
    SDL_EndGPURenderPass(renderPass);
    SDL_SubmitGPUCommandBuffer(cmdbuf);

    SDL_ReleaseGPUTransferBuffer(getGPU(), transferBuffer);
    SDL_ReleaseGPUBuffer(getGPU(), objectDataBuffer);

    for(std::vector<std::byte> test : data)
    {
        test.clear();
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

bool World::runPhysics()
{
    if(physicsTimer.trigger())
    {
        physicsTimer.update();
        if(!GameState::gamePaused() && phyRunning)
        {
            std::lock_guard<std::mutex> lock(usageLock);
            for(Object* object : objects)
            {
                PhysicsObject* phyObj = dynamic_cast<PhysicsObject*>(object);
                if(phyObj != nullptr)
                {
                    phyObj->runPhysics(pps, *this);
                }
            }
        }
        return true;
    }
    return false;
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
} // namespace GameEng