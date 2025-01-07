#include <string>
#include <vector>
#include <cmath>

#include "physicsobject.h"
#include "logging.h"

PhysicsObject::PhysicsObject(SDL_Rect body, int flags, Texture_base* texture) :
    Object(body, texture)
   ,_isStatic(flags & PHYOBJ_STATIC)
   ,_canCollide(flags & PHYOBJ_COLLIDE)
   ,nextBody(body)
   ,currentVelocity()
   ,nextVelocity()
{
    body.x = nextBody.x;
    body.y = nextBody.y;
}

SDL_Rect PhysicsObject::calcDrawBody(double percent, View viewport)
{
    SDL_Rect body = getInterBody(percent);
    body.x = (body.x + viewport.getPosition().x) * viewport.getZoom() + viewport.getZoomXOffset();
    body.y = (body.y - viewport.getPosition().y) * viewport.getZoom() + viewport.getZoomYOffset();
    body.h = round((double)body.h * viewport.getZoom());
    body.w = round((double)body.w * viewport.getZoom());
    return body;
}

SDL_Rect PhysicsObject::getInterBody(double percent)
{
    SDL_Rect interBody;
    interBody.x = body.x + (nextBody.x - body.x) * percent;
    interBody.y = body.y + (nextBody.y - body.y) * percent;
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

bool PhysicsObject::onGround(World& world)
{
    SDL_Rect groundCheck;
    groundCheck.x = body.x;
    groundCheck.y = body.y + body.h;
    groundCheck.w = body.w;
    groundCheck.h = 2;
    std::vector<PhysicsObject*> phyObjects = world.getphyObjects();
    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        if(SDL_HasIntersection(&groundCheck, phyObjects[i]->getBody()))
        {
            return true;
        }
    }
    return false;
}

void PhysicsObject::draw(World* world, double percent, double deltaT)
{

    SDL_Rect body = calcDrawBody(percent, world->getViewpoint());
    Object::draw(world, &body, deltaT);
}

void PhysicsObject::update(double deltaTime, World& world)
{
    if(isStatic())
        return;

    body.x = nextBody.x;
    body.y = nextBody.y;
    currentVelocity.x = nextVelocity.x;
    currentVelocity.y = nextVelocity.y;

    if(!onGround(world))
    {
        velocityDelta(0, world.getGravity() * deltaTime);
    }

    SDL_FPoint displacement;
    displacement.x = ((currentVelocity.x + nextVelocity.x) / 2) * deltaTime;
    displacement.y = ((currentVelocity.y + nextVelocity.y) / 2) * deltaTime;
    std::vector<PhysicsObject*> phyObjects = world.getphyObjects();

    nextBody.x += displacement.x;
    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        if(SDL_HasIntersection(getBody(), phyObjects[i]->getBody()) && this != phyObjects[i])
        {
            collision(phyObjects[i]->getBody());
        }
    }

    nextBody.y += displacement.y;
    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        if(SDL_HasIntersection(getBody(), phyObjects[i]->getBody()) && this != phyObjects[i])
        {
            collision(phyObjects[i]->getBody());
        }
    }
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
                nextBody.x += collisionArea.w;
                body.x += collisionArea.w;
                currentVelocity.x = 0;
                nextVelocity.x = 0;
            }
            else
            {
                nextBody.x -= collisionArea.w;
                body.x -= collisionArea.w;
                currentVelocity.x = 0;
                nextVelocity.x = 0;
            }
        }
        else
        {
            if(other->y < body.y)
            {
                nextBody.y += collisionArea.h;
                body.y += collisionArea.h;
                currentVelocity.y = 0;
                nextVelocity.y = 0;
            }
            else
            {
                nextBody.y -= collisionArea.h;
                body.y -= collisionArea.h;
                currentVelocity.y = 0;
                nextVelocity.y = 0;
            }
        }
    }
}