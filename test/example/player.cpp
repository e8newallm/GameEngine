#include <iostream>
#include <algorithm>

#include "player.h"
#include "keystate.h"

Player::Player(double x, double y, double height, double width, int flags, SDL_Texture* texture) :
    PhysicsObject(x, y, height, width, flags, texture)
{

}

void Player::update(double deltaTime, PhysicsContext* context)
{
    KeyState& keyState = *KeyState::get();

    if(body.y < MAXHEIGHT)
    {
        MAXHEIGHT = body.y;
    }
    if(onGround() && keyState[SDL_SCANCODE_SPACE] == SDL_KEYDOWN)
    {
        SDL_FPoint curVel = getVelocity();
        velocity(curVel.x, -jump);
    }
    
    SDL_FPoint vel = getVelocity();
    if(keyState[SDL_SCANCODE_LEFT] == SDL_KEYDOWN
    || keyState[SDL_SCANCODE_A] == SDL_KEYDOWN)
    {
        velocity(std::min(vel.x, -speed), vel.y);
    }
    else if(keyState[SDL_SCANCODE_RIGHT] == SDL_KEYDOWN
    || keyState[SDL_SCANCODE_D] == SDL_KEYDOWN)
    {
        velocity(std::max(vel.x, speed), vel.y);
    }
    else
    {
        velocity(vel.x/2.0f, vel.y);
    }
    
    PhysicsObject::update(deltaTime, context);
}