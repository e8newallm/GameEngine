#include "object.h"

Object::Object(int x, int y, int height, int width, SDL_Texture* texture) :
    body()
   ,velocity()
   ,tex(texture)

{
    body.x = x;
    body.y = y;
    body.h = height;
    body.w = width;

    SDL_QueryTexture(tex, NULL, NULL, &body.w, &body.h);
}

Object::~Object()
{
    SDL_DestroyTexture(tex);
}

void Object::draw(SDL_Renderer* rend)
{
    SDL_RenderCopy(rend, tex, NULL, &body);
}

void Object::move(int x, int y)
{
    body.x = x;
    body.y = y;
}

void Object::moveDelta(int x, int y)
{
    body.x += x;
    body.y += y;
}

void Object::velosity(int x, int y)
{
    velocity.x = x;
    velocity.y = y;
}

void Object::velosityDelta(int x, int y)
{
    velocity.x += x;
    velocity.y += y;
}

void Object::resize(int height, int width)
{
    body.h = height;
    body.w = width;
}

void Object::resizeDelta(int height, int width)
{
    body.h += height;
    body.w += width;
}