#include <SDL_mutex.h>
#include <SDL_stdinc.h>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <thread>

#include "world.h"
#include "physicsobject.h"
#include "image.h"

#include "gamestate.h"

World::World(SDL_Renderer* rend, View viewport) :
    viewport(viewport)
    , rend(rend)
    , phyRunning(false)
    , lastPhysics(std::chrono::high_resolution_clock::now())
    , usageLock(SDL_CreateMutex())
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

void World::draw(timer lastRender)
{
    SDL_LockMutex(usageLock);

    double deltaTime = std::chrono::duration<double, std::milli>(lastRender - lastPhysics).count();
    double percent = deltaTime / (1000.0f / pps);

    for(int i = UINT8_MAX; i > 128; i--)
    {
        for(Image* image : images[i])
        {
            image->draw(this);
        }
    }

    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        phyObjects[i]->draw(this, percent, deltaTime);
    }

    for(int i = 127; i >= 0; i--)
    {
        for(Image* image : images[i])
        {
            image->draw(this);
        }
    }

    SDL_UnlockMutex(usageLock);
}

void World::update()
{
    SDL_LockMutex(usageLock);
    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        phyObjects[i]->preUpdate();
        phyObjects[i]->update(pps, *this);
    }
    SDL_UnlockMutex(usageLock);
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
    if(((1.0f / pps) - std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - lastPhysics).count() <= 0))
    {
        if(!GameState::gamePaused() && phyRunning) update();

        timer newlastPhysics = std::chrono::high_resolution_clock::now();
        double actualPPS = 1.0f / std::chrono::duration<double>(newlastPhysics - lastPhysics).count();
        std::cout << "Physics: " << actualPPS << "\r\n";
        lastPhysics = newlastPhysics;
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