#ifndef PLAYER_H
#define PLAYER_H

#include "physicsobject.h"

class Player : public PhysicsObject
{
    public:
        Player(double x, double y, double height, double width, int flags, SDL_Texture* texture);
        Player() : PhysicsObject() {};

        virtual void update(double deltaTime);
};

#endif