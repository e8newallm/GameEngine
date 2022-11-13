#include <iostream>

#include "physicsobject.h"
#include "keystate.h"

std::vector<PhysicsObject*> PhysicsObject::collisionObjects, PhysicsObject::noncollisionObjects;

PhysicsObject::PhysicsObject(double x, double y, double height, double width, int flags, SDL_Texture* texture) :
    Object(x, y, height, width, texture)
   ,isStatic(flags & PHYOBJ_STATIC)
   ,canCollide(flags & PHYOBJ_COLLIDE)
{
    if(canCollide)
    {
        std::cout << "ADDING TO COLLIDE\r\n";
        collisionObjects.push_back(this);
    }
    else
    {
        noncollisionObjects.push_back(this);
    }
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::update(double deltaTime)
{    
    if(isStatic)
        return;

    velocityDelta(0, 0.1*deltaTime);

    moveDelta(currentVelocity.x, currentVelocity.y);

    body.x = updateBody.x;
    body.y = updateBody.y;
    body.h = updateBody.h;
    body.w = updateBody.w;
    
}
void PhysicsObject::detectCollision(int pos)
{
    for(int i = pos+1; i < collisionObjects.size(); i++)
    {
        if(SDL_HasIntersection(getBody(), collisionObjects[i]->getBody()))
        {
            collision(collisionObjects[i]->getBody());
            collisionObjects[i]->collision(getBody());
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
            moveDelta(-collisionArea.w, 0);
            currentVelocity.x = 0;
        }
        else
        {
            moveDelta(0, -collisionArea.h);
            currentVelocity.y = 0;
        }
    }
}

void PhysicsObject::updateObjects(double deltaTime, SDL_Renderer* rend)
{
    for(int i = 0; i < collisionObjects.size(); i++)
    {
        collisionObjects[i]->update(deltaTime);
        collisionObjects[i]->detectCollision(i);
        collisionObjects[i]->draw(rend);
    }
    for(int i = 0; i < noncollisionObjects.size(); i++)
    {
        noncollisionObjects[i]->update(deltaTime);
        noncollisionObjects[i]->draw(rend);
    }
}