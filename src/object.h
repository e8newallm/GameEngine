#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Object
{
    public:
        Object(int x, int y, int height, int width, SDL_Texture* texture);
        Object() : Object(0.0f, 0.0f, 0.0f, 0.0f, NULL) {};
        ~Object();

        void draw(SDL_Renderer* rend);
        void update();

        void move(int x, int y);
        void moveDelta(int x, int y);

        void velosity(int x, int y);
        void velosityDelta(int x, int y);

        void resize(int height, int width);
        void resizeDelta(int height, int width);
    //private:
        SDL_Rect body;
        SDL_Point velocity;
        SDL_Texture* tex;
};