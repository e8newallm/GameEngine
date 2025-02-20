#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "object.h"
#include "world.h"

#define PHYOBJ_STATIC   1<<0
#define PHYOBJ_COLLIDE  1<<1

class PhysicsObject : public Object
{
    public:
        PhysicsObject(SDL_Rect body, int flags, Texture_base* texture);
        PhysicsObject() : Object(){};

        using Object::draw;
        using Object::update;
        ShaderObjData predraw() override;
        virtual void draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass, double deltaT) override;
        virtual void update(double deltaTime, World& world) override;

        virtual void runPhysics(double deltaTime, World& world) override;

        bool detectCollision(World& context);
        bool onGround(const World& world) const;

        virtual void velocity(double x, double y);
        virtual void velocityDelta(double x, double y);
        virtual SDL_FPoint getVelocity() { return nextVelocity; };
        virtual const SDL_Rect* getBody() const override { return &interBody; };

        inline bool isStatic() const { return _isStatic; };
        inline bool canCollide() const { return _canCollide; };

    protected:

        bool _isStatic = true;
        bool _canCollide = false;

        SDL_Rect nextBody;
        SDL_Rect interBody;

        SDL_FPoint currentVelocity;
        SDL_FPoint nextVelocity;
};

#endif