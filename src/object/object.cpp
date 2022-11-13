#include <iostream>

#include "object.h"
#include "keystate.h"

Object::Object(double x, double y, double height, double width, SDL_Texture* texture) :
    body()
   ,updateBody()
   ,currentVelocity()
   ,tex(texture)

{
    SDL_QueryTexture(tex, NULL, NULL, NULL, NULL);
    body.x = x;
    body.y = y;
    body.h = height;
    body.w = width;
    
    updateBody.x = x;
    updateBody.y = y;
    updateBody.h = height;
    updateBody.w = width;
}

Object::~Object()
{
    SDL_DestroyTexture(tex);
}

void Object::draw(SDL_Renderer* rend)
{
    SDL_RenderCopy(rend, tex, NULL, &body);
}

void Object::update(double deltaTime)
{
}

void Object::move(double x, double y)
{
    updateBody.x = x;
    updateBody.y = y;
}

void Object::moveDelta(double x, double y)
{
    updateBody.x += x;
    updateBody.y += y;
}

void Object::velocity(double x, double y)
{
    currentVelocity.x = x;
    currentVelocity.y = y;
}

void Object::velocityDelta(double x, double y)
{
    currentVelocity.x += x;
    currentVelocity.y += y;
}

void Object::resize(double height, double width)
{
    updateBody.h = height;
    updateBody.w = width;
}

void Object::resizeDelta(double height, double width)
{
    updateBody.h += height;
    updateBody.w += width;
}