#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <vector>
#include <SDL2/SDL_mutex.h>

#include "object.h"
#include "view.h"

#define PHYOBJ_STATIC 1<<0
#define PHYOBJ_COLLIDE 1<<1

class PhysicsContext;
class PhysicsObject : public Object
{
    public:
        PhysicsObject(double x, double y, double height, double width, 
                      int flags, SDL_Texture* texture);
        PhysicsObject() : Object(){};
        ~PhysicsObject();

        void preUpdate();
        virtual void update(double deltaTime, PhysicsContext* context);
        void draw(SDL_Renderer* rend, double percent, View viewport);

        bool detectCollision(PhysicsContext* context);
        virtual void collision(SDL_Rect* other);
        void groundCheck(PhysicsContext* context);

        virtual void velocity(double x, double y);
        virtual void velocityDelta(double x, double y);
        virtual SDL_FPoint getVelocity() { return nextVelocity; };

        virtual SDL_Rect getInterBody(double percent);
        SDL_Rect calcDrawBody(double percent, View viewport);

        bool onGround() { return _onGround; };
        bool isStatic() { return _isStatic; };
        bool canCollide() { return _canCollide; };

    protected:

        bool _onGround;
        bool _isStatic;
        bool _canCollide;

        SDL_Rect prevBody;

        SDL_FPoint currentVelocity;
        SDL_FPoint nextVelocity;
};

class PhysicsContext
{
    public:
        PhysicsContext();
        void addPhyObj(PhysicsObject* obj);

        void updateObjects(bool instant = false);
        void drawObjects(SDL_Renderer* rend, View viewport);

        double getGravity() { return gravity; };
        void setGravity(double newGravity) { gravity = newGravity; };

        double getPhyTick() { return phyTick; };
        void setPhyTick(double newPhyTick) { phyTick = newPhyTick; };

        std::vector<PhysicsObject*>& getCollisionObjects() { return collisionObjects; };
        std::vector<PhysicsObject*>& getnoncollisionObjects() { return noncollisionObjects; };

    private:
        Uint64 updateTime = 0;
        Uint64 lastRender = 0;
        SDL_mutex* usageLock = SDL_CreateMutex();

        double gravity = 0.00005f;
        double phyTick = 60.0f;

        std::vector<PhysicsObject*> noncollisionObjects;
        std::vector<PhysicsObject*> collisionObjects;
};

#endif