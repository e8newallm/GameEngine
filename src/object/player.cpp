#include "player.h"
#include "keystate.h"

#include <iostream>

Player::Player(double x, double y, double height, double width, int flags, SDL_Texture* texture) :
    PhysicsObject(x, y, height, width, flags, texture)
{

}

void Player::update(double deltaTime)
{
    KeyState& keyState = *KeyState::get();
    const double speed = 0.5;
    const double jump = 1.0;

    if(keyState[SDL_SCANCODE_SPACE] == SDL_KEYDOWN && isOnGround())
    {
        velocityDelta(0, -jump*deltaTime);
    }

    if(keyState[SDL_SCANCODE_LEFT] == SDL_KEYDOWN
    || keyState[SDL_SCANCODE_A] == SDL_KEYDOWN)
    {
        moveDelta(-speed*deltaTime, 0);
    }

    if(keyState[SDL_SCANCODE_RIGHT] == SDL_KEYDOWN
    || keyState[SDL_SCANCODE_D] == SDL_KEYDOWN)
    {
        moveDelta(speed*deltaTime, 0);
    }
    
    PhysicsObject::update(deltaTime);
}