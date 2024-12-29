#include "world.h"
#include "physicsobject.h"
#include <SDL_mutex.h>
#include <cstdint>

int defaultPhysLoop(void* data)
{
    World* context = static_cast<World*>(data);
    context->update();
    while(context->physicsRunning())
    {
        context->update();
    }
    return 0;
}

World::World(SDL_Renderer* rend, View viewport, SDL_ThreadFunction phyFunction) :
    viewport(viewport)
    , rend(rend)
    , phyRunning(false)
    , phyFunction(phyFunction)
    , updateTime(0)
    , lastRender(0)
    , usageLock(SDL_CreateMutex())
    , gravity(0.00005f)
    , phyTick(60.0f)
{

}

World::~World()
{
    for(std::vector<Image*> level : images)
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
    SDL_DestroyMutex(usageLock);
}

void World::draw()
{
    for(int i = UINT8_MAX; i > 128; i--)
    {
        for(Image* image : images[i])
        {
            image->draw(rend);
        }
    }

    drawObjects(rend, viewport);

    for(int i = 127; i >= 0; i--)
    {
        for(Image* image : images[i])
        {
            image->draw(rend);
        }
    }
}

void World::startPhysics()
{
    if(!phyRunning)
    {
        phyRunning = true;
        physThread = SDL_CreateThread(phyFunction, "phyThread", this);
    }
}

void World::stepPhysics()
{
    phyRunning = false;
    if(!phyRunning)
        physThread = SDL_CreateThread(phyFunction, "phyThread", this);
    SDL_WaitThread(physThread, NULL);
}

void World::stopPhysics()
{
    if(phyRunning)
    {
        phyRunning = false;
        SDL_WaitThread(physThread, NULL);
    }
}

void World::addImage(Image* newImage)
{
    this->images[UINT8_MAX].push_back(newImage);
};

void World::drawObjects(SDL_Renderer* rend, View viewport)
{
    SDL_LockMutex(usageLock);
    double deltaTime = (double)((SDL_GetPerformanceCounter() - updateTime)*1000 / (double)SDL_GetPerformanceFrequency());
    //double fps = 1.0f / (double)((SDL_GetPerformanceCounter() - lastRender) / (double)SDL_GetPerformanceFrequency());
    lastRender = SDL_GetPerformanceCounter();
    //std::cout << "\r\nFPS: " << fps << "\r\n";
    double percent = deltaTime / (1000.0f / phyTick);
    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        phyObjects[i]->draw(rend, percent, deltaTime, viewport);
    }
    SDL_UnlockMutex(usageLock);
}

void World::addPhyObj(PhysicsObject* obj)
{
    phyObjects.push_back(obj);
}

void World::update()
{
    SDL_LockMutex(usageLock);
    Uint64 startTime = SDL_GetPerformanceCounter();
    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        phyObjects[i]->groundCheck(*this);
        phyObjects[i]->preUpdate();
        phyObjects[i]->update(phyTick, *this);
    }
    updateTime = SDL_GetPerformanceCounter();
    SDL_UnlockMutex(usageLock);

    double phyTickDuration = (updateTime - startTime) * 1000 / (double)SDL_GetPerformanceFrequency();
    SDL_Delay(std::max((1000.0f / phyTick) - phyTickDuration, 0.0));
}

