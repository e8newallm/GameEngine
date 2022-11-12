#include "object.h"
#include "keystate.h"

#include <iostream>

Object::Object(double x, double y, double height, double width, SDL_Texture* texture) :
    body()
   ,updateBody()
   ,currentVelocity()
   ,tex(texture)

{
    body.x = x;
    body.y = y;
    body.h = height;
    body.w = width;
    
    updateBody.x = x;
    updateBody.y = y;
    updateBody.h = height;
    updateBody.w = width;

    SDL_QueryTexture(tex, NULL, NULL, &body.w, &body.h);
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
    const double speed = 0.0001;
    KeyState& keyState = *KeyState::get();
    if(keyState[SDL_SCANCODE_UP] == SDL_KEYDOWN
    || keyState[SDL_SCANCODE_W] == SDL_KEYDOWN)
    {
        velocityDelta(0, -speed*deltaTime);
    }

    if(keyState[SDL_SCANCODE_DOWN] == SDL_KEYDOWN
    || keyState[SDL_SCANCODE_S] == SDL_KEYDOWN)
    {
        velocityDelta(0, speed*deltaTime);
    }

    if(keyState[SDL_SCANCODE_LEFT] == SDL_KEYDOWN
    || keyState[SDL_SCANCODE_A] == SDL_KEYDOWN)
    {
        velocityDelta(-speed*deltaTime, 0);
    }

    if(keyState[SDL_SCANCODE_RIGHT] == SDL_KEYDOWN
    || keyState[SDL_SCANCODE_D] == SDL_KEYDOWN)
    {
        velocityDelta(speed*deltaTime, 0);
    }
    std::cout << "velocity: " << currentVelocity.x << " " << currentVelocity.y << "\r\n";
    moveDelta(currentVelocity.x, currentVelocity.y);
    body.x = updateBody.x;
    body.y = updateBody.y;
    body.h = updateBody.h;
    body.w = updateBody.w;
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