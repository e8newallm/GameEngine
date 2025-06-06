#include <string>

#include "object.h"
#include "world.h"

namespace GameEng
{
Object::Object(SDL_Rect body, std::shared_ptr<Texture_base> texture) : body(body), tex(std::move(texture))
{
}

Object::~Object() = default;

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
    body.x = static_cast<int>(x);
    body.y = static_cast<int>(y);
}

void Object::moveDelta(double x, double y)
{
    body.x += static_cast<int>(x);
    body.y += static_cast<int>(y);
}

void Object::resize(double height, double width)
{
    body.h = static_cast<int>(height);
    body.w = static_cast<int>(width);
}

void Object::resizeDelta(double height, double width)
{
    body.h += static_cast<int>(height);
    body.w += static_cast<int>(width);
}
} // namespace GameEng