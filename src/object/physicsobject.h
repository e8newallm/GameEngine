#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <vector>
#include <SDL2/SDL_mutex.h>

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
        void draw(SDL_Renderer* rend, double percent);

        virtual void collision(SDL_Rect* other);
        void groundCheck();
        bool isOnGround();

        virtual void velocity(double x, double y);
        virtual void velocityDelta(double x, double y);
        virtual SDL_FPoint getVelocity() { return nextVelocity; };

        virtual SDL_Rect getInterBody(double percent);

        static void updateObjects();
        static void drawObjects(SDL_Renderer* rend);
        bool detectCollision();

    protected:

        bool onGround;
        bool isStatic;
        bool canCollide;

        SDL_Rect prevBody;

        SDL_FPoint currentVelocity;
        SDL_FPoint nextVelocity;

        static std::vector<PhysicsObject*> noncollisionObjects;
        static std::vector<PhysicsObject*> collisionObjects;
        
        static Uint64 updateTime;
        static Uint64 lastRender;
        static SDL_mutex* usageLock;

        static const double gravity;
        static const double phyTick;
};

#endif