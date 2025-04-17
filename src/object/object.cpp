#include "object.h"
#include "world.h"

namespace GameEng
{
    Object::Object(SDL_Rect body, Texture_base* texture) :
        body(body)
    ,tex(texture)
    {

    }

    Object::~Object()
    {
        delete tex;
    }

    void Object::draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass)
    {

        tex->draw(world, buffer, renderPass);

    }

    void Object::update(double deltaTime, World& world)
    {
        (void)world;
        tex->update(deltaTime);
    }

    void Object::move(double x, double y)
    {
        body.x = x;
        body.y = y;
    }

    void Object::moveDelta(double x, double y)
    {
        body.x += x;
        body.y += y;
    }

    void Object::resize(double height, double width)
    {
        body.h = height;
        body.w = width;
    }

    void Object::resizeDelta(double height, double width)
    {
        body.h += height;
        body.w += width;
    }
}