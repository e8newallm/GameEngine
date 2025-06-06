#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "graphics.h"
#include "logging.h"
#include "object.h"
#include "physicsobject.h"

namespace GameEng
{
PhysicsObject::PhysicsObject(SDL_Rect body, int flags, std::shared_ptr<Texture_base> texture)
    : Object(body, std::move(texture)), _isStatic((flags & PhyObjFlag::Static) != 0),
      _canCollide((flags & PhyObjFlag::Collide) != 0), nextBody(body), interBody(body), currentVelocity(),
      nextVelocity()
{
    body.x = nextBody.x;
    body.y = nextBody.y;
}

void PhysicsObject::velocity(double x, double y)
{
    nextVelocity.x = static_cast<float>(x);
    nextVelocity.y = static_cast<float>(y);
}

void PhysicsObject::velocityDelta(double x, double y)
{
    nextVelocity.x += static_cast<float>(x);
    nextVelocity.y += static_cast<float>(y);
}

bool PhysicsObject::onGround(const World& world) const
{
    SDL_Rect groundCheck;
    groundCheck.x = body.x;
    groundCheck.y = body.y + body.h;
    groundCheck.w = body.w;
    groundCheck.h = 2;
    const std::vector<Object*>& objects = world.getObjects();

    return std::ranges::any_of(objects, [&](Object* object) {
        const PhysicsObject* obj = dynamic_cast<PhysicsObject*>(object);
        return obj != nullptr && obj != this && SDL_HasRectIntersection(&groundCheck, object->getBody());
    });
}

std::vector<std::byte> PhysicsObject::predraw()
{
    struct ObjData
    {
        SDL_Rect body;
        SDL_FRect texBody;
    };

    std::vector<std::byte> mem(sizeof(ObjData));
    ObjData* data = reinterpret_cast<ObjData*>(mem.data());
    data->body = body;
    data->texBody = tex->getUV();
    return mem;
}

void PhysicsObject::update(double deltaTime, World& world)
{
    interBody.x = static_cast<int>(body.x + ((nextBody.x - body.x) * world.getPhyInterpolation()));
    interBody.y = static_cast<int>(body.y + ((nextBody.y - body.y) * world.getPhyInterpolation()));
    interBody.h = body.h;
    interBody.w = body.w;

    Object::update(deltaTime, world);
}

void PhysicsObject::runPhysics(double deltaTime, World& world)
{
    if(isStatic())
    {
        return;
    }

    body.x = nextBody.x;
    body.y = nextBody.y;
    currentVelocity.x = nextVelocity.x;
    currentVelocity.y = nextVelocity.y;

    if(!onGround(world))
    {
        velocityDelta(0, world.getGravity() * deltaTime);
    }

    SDL_FPoint displacement;
    displacement.x = static_cast<float>(((currentVelocity.x + nextVelocity.x) / 2) * deltaTime);
    displacement.y = static_cast<float>(((currentVelocity.y + nextVelocity.y) / 2) * deltaTime);
    const std::vector<Object*>& objects = world.getObjects();
    SDL_Rect collisionArea;

    nextBody.x += static_cast<int>(displacement.x);
    for(Object* object : objects)
    {
        const PhysicsObject* obj = dynamic_cast<PhysicsObject*>(object);

        if(obj != nullptr && SDL_GetRectIntersection(&nextBody, obj->getBody(), &collisionArea) && this != object)
        {
            if(obj->getBody()->x < body.x)
            {
                nextBody.x += collisionArea.w;
            }
            else
            {
                nextBody.x -= collisionArea.w;
            }
            currentVelocity.x = 0;
            nextVelocity.x = 0;
        }
    }

    nextBody.y += static_cast<int>(displacement.y);
    for(Object* object : objects)
    {
        const PhysicsObject* obj = dynamic_cast<PhysicsObject*>(object);
        if(obj != nullptr && SDL_GetRectIntersection(&nextBody, obj->getBody(), &collisionArea) && this != object)
        {
            if(obj->getBody()->y < body.y)
            {
                nextBody.y += collisionArea.h;
            }
            else
            {
                nextBody.y -= collisionArea.h;
            }
            currentVelocity.y = 0;
            nextVelocity.y = 0;
        }
    }
}
} // namespace GameEng