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

    std::cout << "y: " << body.y << "\r\n";
    if(body.y < MAXHEIGHT)
    {
        MAXHEIGHT = body.y;
    }
    std::cout << "MAXHEIGHT: " << MAXHEIGHT << "\r\n";
    if(true && isOnGround()) //keyState[SDL_SCANCODE_SPACE] == SDL_KEYDOWN
    {
        std::cout << "JUMOP\r\n";
        velocityDelta(0, -jump);
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