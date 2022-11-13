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

        virtual void update(double deltaTime);
        virtual void collision(SDL_Rect* other);

        static void updateObjects(double deltaTime, SDL_Renderer* rend);
        void detectCollision(int pos);

    protected:
        bool isStatic;
        bool canCollide;

        static std::vector<PhysicsObject*> noncollisionObjects;
        static std::vector<PhysicsObject*> collisionObjects;
};

#endif