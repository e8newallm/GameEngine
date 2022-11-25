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
    getPhysicsContext()->drawObjects(rend, *viewport);
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