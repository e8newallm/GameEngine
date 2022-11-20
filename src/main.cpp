#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_thread.h>

#include <iostream>
#include <ctime>

#include "object.h"
#include "physicsobject.h"
#include "player.h"

#include "keystate.h"

bool close = false;

int physLoop(void* data)
{
    while(!close)
    {
        PhysicsObject::updateObjects();
    }
    return 0;
}

int main()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1000, 1000, 0);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    std::srand(std::time(nullptr));
    KeyState* keyState = KeyState::get();

    PhysicsObject test(0.0, 960.0, 40.0, 1000.0, PHYOBJ_STATIC | PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png")));
    PhysicsObject testTwo(0.0, 900.0, 60.0, 500.0, PHYOBJ_STATIC | PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png")));
    PhysicsObject testThree(700.0, 900.0, 60.0, 500.0, PHYOBJ_STATIC | PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png")));
    PhysicsObject testFour(700.0, 600.0, 60.0, 200.0, PHYOBJ_STATIC | PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png")));
    Player player(500.0, 920.0, 40.0, 40.0, PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png")));
    
    SDL_Thread* physThread = SDL_CreateThread(physLoop, "phyThread", NULL);

    while (!close)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    close = 1;
                    break;
                }
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                {
                    keyState->update(event.key.keysym.scancode, event.type);
                    break;
                }
                default:
                {
                       
                }
            }
        }

        SDL_RenderClear(rend);
        PhysicsObject::drawObjects(rend);
        SDL_RenderPresent(rend);
        SDL_Delay(10);
    }
    
    SDL_WaitThread(physThread, NULL);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
 
    return 0;
}