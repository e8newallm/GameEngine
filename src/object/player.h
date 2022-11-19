#ifndef PLAYER_H
#define PLAYER_H

#include "physicsobject.h"

class Player : public PhysicsObject
{
    public:
        Player(double x, double y, double height, double width, int flags, SDL_Texture* texture);
        Player() : PhysicsObject() {};

        virtual void update(double deltaTime);

    private:
        const double speed = 0.5f;
        const double jump = 2.0f;
        double MAXHEIGHT = 99999.0f;
};

#endif