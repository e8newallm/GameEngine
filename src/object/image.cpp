#include "image.h"
#include "texture.h"

Image::Image(SDL_Rect body, std::string texture, Uint8 layer) :
    Object(body, new Texture(texture))
    , layer(layer)
{
}


void Image::draw(SDL_Renderer* rend, double deltaT)
{
    draw(rend, &body, deltaT);
}

void Image::draw(SDL_Renderer* rend, SDL_Rect* bodyPos, double deltaT)
{
    tex->update(deltaT);
    tex->draw(rend, bodyPos);
}
