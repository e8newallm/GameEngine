#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "object.h"

int main()
{
 
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_Window* win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1000, 1000, 0);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    Object test(500.0, 500.0, 10.0, 10.0, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png")));

 
    // controls animation loop
    int close = 0;
 
    // speed of box
    int speed = 300;
 
    // animation loop
    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
 
            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;
 
            case SDL_KEYDOWN:
                // keyboard API for key pressed
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    test.moveDelta(0, -speed / 30);
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    test.moveDelta(-speed / 30, 0);
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    test.moveDelta(0, speed / 30);
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    test.moveDelta(speed / 30, 0);
                    break;
                default:
                    break;
                }
            }
        }
 
        SDL_RenderClear(rend);

        test.draw(rend);

        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
    }
 
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
 
    return 0;
}