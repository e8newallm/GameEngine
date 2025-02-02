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

    std::vector<ShaderObjData> data{};
    std::vector<Uint32> indexes{};
    Uint32 dataSize = 0;

    for(int i = UINT8_MAX; i > 128; i--)
    {
        for(Image* image : images[i])
        {
            ShaderObjData newData = image->predraw();
            indexes.push_back(dataSize);
            dataSize += newData.size;
            data.push_back(newData);
        }
    }

    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        ShaderObjData newData = phyObjects[i]->predraw();
        indexes.push_back(dataSize);
        dataSize += newData.size;
        data.push_back(newData);
    }

    for(int i = 127; i >= 0; i--)
    {
        for(Image* image : images[i])
        {
            ShaderObjData newData = image->predraw();
            indexes.push_back(dataSize);
            dataSize += newData.size;
            data.push_back(newData);
        }
    }

    //FUTURE FUNCTION

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer* objectDataBuffer;

    transferBuffer = SDL_CreateGPUTransferBuffer(
		gpu,
		&(SDL_GPUTransferBufferCreateInfo) {
			.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
			.size = dataSize + (Uint32)indexes.size() * (Uint32)sizeof(Uint32)
		}
	);

    objectDataBuffer = SDL_CreateGPUBuffer(
		gpu,
		&(SDL_GPUBufferCreateInfo) {
			.usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
			.size = dataSize + (Uint32)indexes.size() * (Uint32)sizeof(Uint32)
		}
	);

    Uint32* dataPtr = (Uint32*)SDL_MapGPUTransferBuffer(
        gpu,
        transferBuffer,
        true
	);

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

    SDL_UploadToGPUBuffer(
        copyPass,
        &(SDL_GPUTransferBufferLocation) {
            .transfer_buffer = transferBuffer,
            .offset = 0
        },
        &(SDL_GPUBufferRegion) {
            .buffer = objectDataBuffer,
            .offset = 0,
            .size = dataSize + (Uint32)indexes.size() * 4
        },
        true
    );

    SDL_EndGPUCopyPass(copyPass);

    // END OF FUTURE FUNCTION

    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, NULL);

    ShaderWorldData worldData {{0, 0, 1000, 1000}}; //*getView().window()
	SDL_PushGPUVertexUniformData(cmdbuf, 1, &worldData, sizeof(ShaderWorldData));

    int drawID = 0;
    for(int i = UINT8_MAX; i > 128; i--)
    {
        for(Image* image : images[i])
        {
            SDL_PushGPUVertexUniformData(cmdbuf, 0, &drawID, sizeof(Uint32));
            drawID++;
            image->draw(this, objectDataBuffer, renderPass);
        }
    }

    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        SDL_PushGPUVertexUniformData(cmdbuf, 0, &drawID, sizeof(Uint32));
        drawID++;
        phyObjects[i]->draw(this, objectDataBuffer, renderPass, percent, deltaTime);
    }

    for(int i = 127; i >= 0; i--)
    {
        for(Image* image : images[i])
        {
            SDL_PushGPUVertexUniformData(cmdbuf, 0, &drawID, sizeof(Uint32));
            drawID++;
            image->draw(this, objectDataBuffer, renderPass);
        }
    }

    SDL_EndGPURenderPass(renderPass);
    SDL_SubmitGPUCommandBuffer(cmdbuf);

    for(ShaderObjData test : data)
        free(test.data);
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