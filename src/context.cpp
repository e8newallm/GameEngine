#include <iostream>

#include "context.h"

int defaultPhysLoop(void* data)
{
    Context* context = static_cast<Context*>(data);
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
    , phyRunning(true)
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