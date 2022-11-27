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
    for(Image* image : backgroundImages)
        image->draw(rend);

    getPhysicsContext()->drawObjects(rend, *viewport);

    for(Image* image : foregroundImages)
        image->draw(rend);
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
    if(newImage->isForeground())
    {
        this->foregroundImages.push_back(newImage);
    }
    else
    {
        this->backgroundImages.push_back(newImage);
    }
    
};