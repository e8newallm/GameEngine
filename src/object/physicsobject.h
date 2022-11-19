#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <vector>

#include "object.h"

#define PHYOBJ_STATIC 1<<0
#define PHYOBJ_COLLIDE 1<<1

class PhysicsObject : public Object
{
    public:
        PhysicsObject(double x, double y, double height, double width, 
                      int flags, SDL_Texture* texture);
        PhysicsObject() : Object(){};
        ~PhysicsObject();

        void preUpdate();
        virtual void update(double deltaTime);
        virtual void collision(SDL_Rect* other);
        void groundCheck();
        bool isOnGround();

        virtual void velocity(double x, double y);
        virtual void velocityDelta(double x, double y);

        static void updateObjects(double deltaTime, SDL_Renderer* rend);
        bool detectCollision();

    protected:

        const double gravity = 0.005f;

        bool onGround;
        bool isStatic;
        bool canCollide;

        SDL_FPoint currentVelocity;
        SDL_FPoint nextVelocity;

        static std::vector<PhysicsObject*> noncollisionObjects;
        static std::vector<PhysicsObject*> collisionObjects;
};

#endif