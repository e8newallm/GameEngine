#include <iostream>

#include "object.h"
#include "keystate.h"

Object::Object(SDL_Rect body, SDL_Texture* texture) :
    body(body)
   ,updateBody()
   ,tex(texture)
{
    SDL_QueryTexture(tex, NULL, NULL, NULL, NULL);
    
    updateBody.x = body.x;
    updateBody.y = body.y;
    updateBody.h = body.h;
    updateBody.w = body.w;
}

Object::~Object()
{
    SDL_DestroyTexture(tex);
}

void Object::draw(SDL_Renderer* rend, SDL_Rect* texturePos)
{
    draw(rend, &body, texturePos);
}

void Object::draw(SDL_Renderer* rend, SDL_Rect* bodyPos, SDL_Rect* texturePos)
{
    SDL_RenderCopyEx(rend, tex, texturePos, bodyPos, 0.0, NULL, SDL_FLIP_NONE);
}

void Object::update(double deltaTime)
{
}

void Object::move(double x, double y)
{
    updateBody.x = x;
    updateBody.y = y;
}

void Object::moveInstantly(double x, double y)
{
    body.x = x;
    body.y = y;
}


void Object::moveDelta(double x, double y)
{
    updateBody.x += x;
    updateBody.y += y;
}

void Object::moveDeltaInstantly(double x, double y)
{
    body.x += x;
    body.y += y;
}

void Object::resize(double height, double width)
{
    updateBody.h = height;
    updateBody.w = width;
}

void Object::resizeDelta(double height, double width)
{
    updateBody.h += height;
    updateBody.w += width;
}