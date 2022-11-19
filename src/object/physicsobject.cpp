#include <iostream>
#include <cmath>

#include "physicsobject.h"
#include "keystate.h"

std::vector<PhysicsObject*> PhysicsObject::collisionObjects, PhysicsObject::noncollisionObjects;

PhysicsObject::PhysicsObject(double x, double y, double height, double width, int flags, SDL_Texture* texture) :
    Object(x, y, height, width, texture)
   ,isStatic(flags & PHYOBJ_STATIC)
   ,canCollide(flags & PHYOBJ_COLLIDE)
   ,currentVelocity()
   ,nextVelocity()
{
    if(canCollide)
    {
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

void PhysicsObject::groundCheck()
{
    SDL_Rect groundCheck;
    groundCheck.x = body.x;
    groundCheck.y = body.y + body.h;
    groundCheck.w = body.w;
    groundCheck.h = 2;
    for(int i = 0; i < collisionObjects.size(); i++)
    {
        if(SDL_HasIntersection(&groundCheck, collisionObjects[i]->getBody()))
        {
            onGround = true;
            return;
        }
    }
    onGround = false;
    return;
}

void PhysicsObject::preUpdate()
{
    currentVelocity.x = nextVelocity.x;
    currentVelocity.y = nextVelocity.y;
}

bool PhysicsObject::isOnGround()
{
    return onGround;
}

void PhysicsObject::update(double deltaTime)
{    
    if(isStatic)
        return;

    if(!isOnGround())
        velocityDelta(0, gravity * deltaTime);

    SDL_FPoint displacement;
    displacement.x = ((currentVelocity.x + nextVelocity.x) / 2) * deltaTime;
    displacement.y = ((currentVelocity.y + nextVelocity.y) / 2) * deltaTime;
    std::cout << "currentVel: " << currentVelocity.x << " " << currentVelocity.y << "\r\n";
    std::cout << "nextVel: " << nextVelocity.x << " " << nextVelocity.y << "\r\n";
    std::cout << "displacement: " << displacement.x << " " << displacement.y << "\r\n";

    float magnitude = sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
    std::cout << "MAGNATUDE: " << magnitude << " / DELTA: " << magnitude / deltaTime << "\r\n";
    if(magnitude > 5.0f)
    {
        SDL_FPoint tempDisplacement;
        tempDisplacement.x = (displacement.x / magnitude) * 5.0f;
        tempDisplacement.y = (displacement.y / magnitude) * 5.0f;
        for(int i = 0; i < magnitude / 5.0f; i++)
        {
            moveDelta(tempDisplacement.x, tempDisplacement.y);
            body.x = updateBody.x;
            body.y = updateBody.y;
            body.h = updateBody.h;
            body.w = updateBody.w;
            if(detectCollision())
                return;
        }
    }
    else
    {
        moveDelta(displacement.x, displacement.y);
        body.x = updateBody.x;
        body.y = updateBody.y;
        body.h = updateBody.h;
        body.w = updateBody.w;
        detectCollision();
    }
    std::cout << "body.x: " << body.x << " body.y: " << body.y << "\r\n";
}

bool PhysicsObject::detectCollision()
{
    for(int i = 0; i < collisionObjects.size(); i++)
    {
        if(SDL_HasIntersection(getBody(), collisionObjects[i]->getBody()) && this != collisionObjects[i])
        {
            collision(collisionObjects[i]->getBody());
            return true;
        }
    }
    return false;
}

void PhysicsObject::collision(SDL_Rect* other)
{
    SDL_Rect collisionArea;
    if(SDL_IntersectRect(getBody(), other, &collisionArea))
    {
        if(collisionArea.h > collisionArea.w)
        {
            body.x -= collisionArea.w;
            currentVelocity.x = 0;
            nextVelocity.x = 0;
        }
        else
        {
            body.y -= collisionArea.h;
            currentVelocity.y = 0;
            nextVelocity.y = 0;
        }
    }
}

void PhysicsObject::updateObjects(double deltaTime, SDL_Renderer* rend)
{
    for(int i = 0; i < collisionObjects.size(); i++)
    {
        collisionObjects[i]->groundCheck();
        collisionObjects[i]->preUpdate();
        collisionObjects[i]->update(deltaTime);
        collisionObjects[i]->draw(rend);
    }
    for(int i = 0; i < noncollisionObjects.size(); i++)
    {
        collisionObjects[i]->preUpdate();
        noncollisionObjects[i]->update(deltaTime);
        noncollisionObjects[i]->draw(rend);
    }
}