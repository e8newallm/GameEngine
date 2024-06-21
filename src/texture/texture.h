#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

struct TexRequest
{
    SDL_Texture* texture;
    SDL_Rect position;
};

class Texture
{
    public:
        Texture(SDL_Renderer* rend);
        Texture(SDL_Renderer* rend, const char* texturePath);
        Texture(SDL_Renderer* rend, std::vector<uint8_t> data);
        virtual TexRequest getTexture();
        virtual void update(double deltaT) {(void) deltaT;};
    protected:
        SDL_Texture* texture;
};

#endif