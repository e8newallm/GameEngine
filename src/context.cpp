#include <iostream>

#include "context.h"

int defaultPhysLoop(void* data)
{
    Context* context = static_cast<Context*>(data);
    context->getPhysicsContext()->updateObjects();
    while(context->physicsRunning())
    {
        context->getPhysicsContext()->updateObjects();
    }
    return 0;
}

Context::Context(SDL_Renderer* rend, View* viewport, SDL_ThreadFunction phyFunction) :
    viewport(viewport)
    , rend(rend)
    , keyState(KeyState::get())
    , phyRunning(false)
    , phyFunction(phyFunction)
{
    phyContext = new PhysicsContext();
    if(phyFunction == nullptr)
    {
        this->phyFunction = defaultPhysLoop;
    }
}

void Context::draw()
{
    for(int i = UINT8_MAX; i > 128; i--)
    {
        for(Image* image : images[i])
        {
            image->draw(rend);
        }
    }

    getPhysicsContext()->drawObjects(rend, *viewport);

    for(int i = 127; i >= 0; i--)
    {
        for(Image* image : images[i])
        {
            image->draw(rend);
        }
    }
}

void Context::startPhysics()
{
    phyRunning = true; 
    physThread = SDL_CreateThread(phyFunction, "phyThread", this);
}

void Context::stepPhysics()
{
    phyRunning = false;
    physThread = SDL_CreateThread(phyFunction, "phyThread", this);
    SDL_WaitThread(physThread, NULL);
}

void Context::stopPhysics()
{ 
    phyRunning = false;
    SDL_WaitThread(physThread, NULL);
}

void Context::addImage(Image* newImage)
{
    this->images[newImage->getLayer()].push_back(newImage);    
};