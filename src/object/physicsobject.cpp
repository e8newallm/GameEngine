#include <iostream>
#include <cmath>

#include "physicsobject.h"
#include "keystate.h"

PhysicsObject::PhysicsObject(double x, double y, double height, double width, int flags, SDL_Texture* texture) :
    Object(x, y, height, width, texture)
   ,_isStatic(flags & PHYOBJ_STATIC)
   ,_canCollide(flags & PHYOBJ_COLLIDE)
   ,currentVelocity()
   ,nextVelocity()
   ,prevBody()
{
    prevBody.x = body.x;
    prevBody.y = body.y;
}

PhysicsObject::~PhysicsObject()
{
}

SDL_Rect PhysicsObject::getInterBody(double percent)
{
    SDL_Rect interBody;
    interBody.x = prevBody.x + (body.x - prevBody.x) * percent;
    interBody.y = prevBody.y + (body.y - prevBody.y) * percent;
    interBody.h = body.h;
    interBody.w = body.w;
    return interBody;
}

void PhysicsObject::velocity(double x, double y)
{
    nextVelocity.x = x;
    nextVelocity.y = y;
}

void PhysicsObject::velocityDelta(double x, double y)
{
    nextVelocity.x += x;
    nextVelocity.y += y;
}

void PhysicsObject::groundCheck(PhysicsContext* context)
{
    SDL_Rect groundCheck;
    groundCheck.x = body.x;
    groundCheck.y = body.y + body.h;
    groundCheck.w = body.w;
    groundCheck.h = 2;
    std::vector<PhysicsObject*> collisionObjects = context->getCollisionObjects();
    for(int i = 0; i < collisionObjects.size(); i++)
    {
        if(SDL_HasIntersection(&groundCheck, collisionObjects[i]->getBody()))
        {
            _onGround = true;
            return;
        }
    }
    _onGround = false;
    return;
}

void PhysicsObject::preUpdate()
{
    prevBody.x = body.x;
    prevBody.y = body.y;
    currentVelocity.x = nextVelocity.x;
    currentVelocity.y = nextVelocity.y;
}

void PhysicsObject::draw(SDL_Renderer* rend, double percent, View viewport)
{
    SDL_Rect body = getInterBody(percent);
    body.x = (body.x + viewport.getPosition().x) * viewport.getZoom() + viewport.getZoomXOffset();
    body.y = (body.y - viewport.getPosition().y) * viewport.getZoom() + viewport.getZoomYOffset();
    body.h = body.h * viewport.getZoom();
    body.w = body.w * viewport.getZoom();
    SDL_RenderCopy(rend, tex, NULL, &body);
}

void PhysicsObject::update(double deltaTime, PhysicsContext* context)
{    
    if(isStatic())
        return;

    if(!onGround())
        velocityDelta(0, context->getGravity() * deltaTime);

    SDL_FPoint displacement;
    displacement.x = ((currentVelocity.x + nextVelocity.x) / 2) * deltaTime;
    displacement.y = ((currentVelocity.y + nextVelocity.y) / 2) * deltaTime;

    float magnitude = sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
    if(magnitude > 5.0f)
    {
        SDL_FPoint tempDisplacement;
        tempDisplacement.x = (displacement.x / magnitude) * 5.0f;
        tempDisplacement.y = (displacement.y / magnitude) * 5.0f;
        int loops = magnitude / 5.0f;
        double remainder = magnitude / 5.0f - loops;
        for(int i = 0; i < loops; i++)
        {
            moveDelta(tempDisplacement.x, tempDisplacement.y);
            body.x = updateBody.x;
            body.y = updateBody.y;
            body.h = updateBody.h;
            body.w = updateBody.w;
            detectCollision(context);
        }
        moveDelta((displacement.x / magnitude) * remainder * 5.0f, (displacement.y / magnitude) * remainder * 5.0f);
        body.x = updateBody.x;
        body.y = updateBody.y;
        body.h = updateBody.h;
        body.w = updateBody.w;
        detectCollision(context);
    }
    else
    {
        moveDelta(displacement.x, displacement.y);
        body.x = updateBody.x;
        body.y = updateBody.y;
        body.h = updateBody.h;
        body.w = updateBody.w;
        detectCollision(context);
    }
}

bool PhysicsObject::detectCollision(PhysicsContext* context)
{
    bool collisionFound = false;
    std::vector<PhysicsObject*> collisionObjects = context->getCollisionObjects();
    for(int i = 0; i < collisionObjects.size(); i++)
    {
        if(SDL_HasIntersection(getBody(), collisionObjects[i]->getBody()) && this != collisionObjects[i])
        {
            collision(collisionObjects[i]->getBody());
            collisionFound = true;
        }
    }
    return collisionFound;
}

void PhysicsObject::collision(SDL_Rect* other)
{
    SDL_Rect collisionArea;
    if(SDL_IntersectRect(getBody(), other, &collisionArea))
    {
        if(collisionArea.h > collisionArea.w)
        {
            if(other->x < body.x)
            {
                moveDelta(collisionArea.w, 0);
                body.x += collisionArea.w;
                if(nextVelocity.x < 0.0f)
                {
                    currentVelocity.x = 0;
                    nextVelocity.x = 0;
                }
            }
            else
            {
                moveDelta(-collisionArea.w, 0);
                body.x -= collisionArea.w;
                if(nextVelocity.x > 0.0f)
                {
                    currentVelocity.x = 0;
                    nextVelocity.x = 0;
                }
            }
        }
        else
        {
            if(other->y < body.y)
            {
                moveDelta(0, collisionArea.h);
                body.y += collisionArea.h;
                if(nextVelocity.y < 0.0f)
                {
                    currentVelocity.y = 0;
                    nextVelocity.y = 0;
                }
            }
            else
            {
                moveDelta(0, -collisionArea.h);
                body.y -= collisionArea.h;
                if(nextVelocity.y > 0.0f)
                {
                    currentVelocity.y = 0;
                    nextVelocity.y = 0;
                }
            }
        }
    }
}

PhysicsContext::PhysicsContext() :
    updateTime(0)
    , lastRender(0)
    , usageLock(SDL_CreateMutex())
    , gravity(0.00005f)
    , phyTick(60.0f)
{

}

void PhysicsContext::addPhyObj(PhysicsObject* obj)
{
    if(obj->canCollide())
    {
        collisionObjects.push_back(obj);
    }
    else
    {
        noncollisionObjects.push_back(obj);
    }
}

void PhysicsContext::updateObjects()
{
    SDL_LockMutex(usageLock);
    Uint64 startTime = SDL_GetPerformanceCounter();
    for(int i = 0; i < collisionObjects.size(); i++)
    {
        collisionObjects[i]->groundCheck(this);
        collisionObjects[i]->preUpdate();
        collisionObjects[i]->update(phyTick, this);
    }
    for(int i = 0; i < noncollisionObjects.size(); i++)
    {
        collisionObjects[i]->preUpdate();
        noncollisionObjects[i]->update(phyTick, this);
    }
    updateTime = SDL_GetPerformanceCounter();
    SDL_UnlockMutex(usageLock);

    double phyTickDuration = (updateTime - startTime) * 1000 / (double)SDL_GetPerformanceFrequency();
    
    SDL_Delay(std::max((1000.0f / phyTick) - phyTickDuration, 0.0));
}

void PhysicsContext::drawObjects(SDL_Renderer* rend, View viewport)
{
    SDL_LockMutex(usageLock);
    double deltaTime = (double)((SDL_GetPerformanceCounter() - updateTime)*1000 / (double)SDL_GetPerformanceFrequency());
    double fps = 1.0f / (double)((SDL_GetPerformanceCounter() - lastRender) / (double)SDL_GetPerformanceFrequency());
    lastRender = SDL_GetPerformanceCounter();
    //std::cout << "\r\nFPS: " << fps << "\r\n";
    double percent = deltaTime / (1000.0f / phyTick);
    for(int i = 0; i < collisionObjects.size(); i++)
    {
        collisionObjects[i]->draw(rend, percent, viewport);
    }

    for(int i = 0; i < noncollisionObjects.size(); i++)
    {
        noncollisionObjects[i]->draw(rend, percent, viewport);
    }
    SDL_UnlockMutex(usageLock);
}