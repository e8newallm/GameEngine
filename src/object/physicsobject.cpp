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
    SDL_Rect interBody = getInterBody(percent);
    interBody.x = (interBody.x - viewport.window()->x) * viewport.getZoom();
    interBody.y = (interBody.y - viewport.window()->y) * viewport.getZoom();
    interBody.h = round((double)interBody.h * viewport.getZoom());
    interBody.w = round((double)interBody.w * viewport.getZoom());
    return interBody;
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
    //std::cout << "percent: " << percent << "\r\n";
    SDL_Rect body = calcDrawBody(percent, world->getView());
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
        if(SDL_HasIntersection(&nextBody, &phyObjects[i]->nextBody) && this != phyObjects[i])
        {
            collision(&phyObjects[i]->nextBody);
        }
    }

    nextBody.y += displacement.y;
    for(uint64_t i = 0; i < phyObjects.size(); i++)
    {
        if(SDL_HasIntersection(&nextBody, &phyObjects[i]->nextBody) && this != phyObjects[i])
        {
            collision(&phyObjects[i]->nextBody);
        }
    }
}

void PhysicsObject::collision(SDL_Rect* other)
{
    SDL_Rect collisionArea;
    if(SDL_IntersectRect(&nextBody, other, &collisionArea))
    {
        //std::cout << "collision!\r\n";
        if(collisionArea.h > collisionArea.w)
        {
            if(other->x < body.x)
            {
                nextBody.x += collisionArea.w;
            }
            else
            {
                nextBody.x -= collisionArea.w;
            }
            currentVelocity.x = 0;
            nextVelocity.x = 0;
        }
        else
        {
            if(other->y < body.y)
            {
                nextBody.y += collisionArea.h;
            }
            else
            {
                nextBody.y -= collisionArea.h;
            }
            currentVelocity.y = 0;
            nextVelocity.y = 0;
        }
        //std::cout << "new body: " << body.x << ", " << body.y << " - " << body.w << ", " << body.h << "\r\n";
        //std::cout << "new nextBody: " << nextBody.x << ", " << nextBody.y << " - " << nextBody.w << ", " << nextBody.h << "\r\n";
    }
}