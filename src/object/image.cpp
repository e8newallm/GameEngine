#include "image.h"
#include "graphics.h"
#include "texture.h"
#include <string>

Image::Image(SDL_Rect body, const std::string& texture) :
    Object(body, new Texture(texture))
{
}


void Image::draw(World* world, SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass, double deltaT)
{
    draw(world, cmdbuf, renderPass, ShaderObjData{body}, deltaT);
}

void Image::draw(World* world, SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass, ShaderObjData objData, double deltaT)
{
    tex->update(deltaT);
    tex->draw(world,cmdbuf, renderPass, objData);
}
