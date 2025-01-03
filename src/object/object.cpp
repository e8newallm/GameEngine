#include "object.h"
#include "world.h"

Object::Object(SDL_Rect body, Texture_base* texture) :
    body(body)
   ,tex(texture)
{

}

Object::~Object()
{
    delete tex;
}

void Object::draw(World* world, double deltaT)
{
    draw(world, &body, deltaT);
}

void Object::draw(World* world, SDL_Rect* bodyPos, double deltaT)
{
    tex->update(deltaT);
    tex->draw(world, bodyPos);

}
void Object::update(double deltaTime)
{
    (void)deltaTime;
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