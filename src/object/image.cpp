#include "image.h"
#include "texture.h"

Image::Image(SDL_Rect body, std::string texture) :
    Object(body, new Texture(texture))
{
}


void Image::draw(World* world, double deltaT)
{
    draw(world, &body, deltaT);
}

void Image::draw(World* world, SDL_Rect* bodyPos, double deltaT)
{
    tex->update(deltaT);
    tex->draw(world, bodyPos);
}
