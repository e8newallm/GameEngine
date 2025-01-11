#include <SDL3/SDL.h>
#include <cstdint>
#include <iostream>

#include "world.h"
#include "physicsobject.h"
#include "image.h"

#include "gamestate.h"

double PPS = 0;

World::World(SDL_Renderer* rend, View viewport) :
    viewport(viewport)
    , rend(rend)
    , phyRunning(false)
    , lastPhysics()
    , usageLock(SDL_CreateMutex())
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
    SDL_DestroyMutex(usageLock);
}

void World::draw(double deltaTime)
{
    SDL_LockMutex(usageLock);

    double percent = lastPhysics.getElapsed() / (1000.0f / pps);

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