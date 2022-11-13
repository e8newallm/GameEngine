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
    const double jump = 2.0;

    std::cout << "DELTA: " << deltaTime << "\r\n";

    if(keyState[SDL_SCANCODE_UP] == SDL_KEYDOWN
    || keyState[SDL_SCANCODE_W] == SDL_KEYDOWN)
    {
        SDL_Point groundCheck;
        groundCheck.x = body.x + body.w/2;
        groundCheck.y = body.y + body.h + 2;
        for(int i = 0; i < collisionObjects.size(); i++)
        {
            if(SDL_PointInRect(&groundCheck, collisionObjects[i]->getBody()))
            {
                velocityDelta(0, -jump*deltaTime);
                break;
            }
        }
        
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