#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "object.h"
#include "view.h"
#include "world.h"

#define PHYOBJ_STATIC 1<<0
#define PHYOBJ_COLLIDE 1<<1

class PhysicsObject : public Object
{
    public:
        PhysicsObject(SDL_Rect body, int flags, Texture_base* texture);
        PhysicsObject() : Object(){};

        using Object::draw;
        using Object::update;
        virtual void draw(World* world, double percent, double deltaT);
        virtual void update(double deltaTime, World& world);

        bool detectCollision(World& context);
        virtual void collision(SDL_Rect* other);
        bool onGround(World& context);

        virtual void velocity(double x, double y);
        virtual void velocityDelta(double x, double y);
        virtual SDL_FPoint getVelocity() { return nextVelocity; };

        virtual SDL_Rect getInterBody(double percent);
        SDL_Rect calcDrawBody(double percent, View viewport);

        bool isStatic() { return _isStatic; };
        bool canCollide() { return _canCollide; };

    protected:

        bool _isStatic = true;
        bool _canCollide = false;

        SDL_Rect nextBody;

        SDL_FPoint currentVelocity;
        SDL_FPoint nextVelocity;
};

#endif