#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_thread.h>

#include <iostream>
#include <ctime>

#include "object.h"
#include "physicsobject.h"
#include "context.h"
#include "player.h"

#include "keystate.h"

bool close = false;

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
    KeyState* keyState = KeyState::get();

    View viewport( {1000, 1000}, {0, 0});
    viewport.setZoom(1.0);

    Context state(rend, &viewport);

    PhysicsContext* phyContext = state.getPhysicsContext();
    phyContext->addPhyObj(new PhysicsObject(0.0, 960.0, 40.0, 1000.0, PHYOBJ_STATIC | PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png"))));
    phyContext->addPhyObj(new PhysicsObject(0.0, 900.0, 60.0, 500.0, PHYOBJ_STATIC | PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png"))));
    phyContext->addPhyObj(new PhysicsObject(700.0, 900.0, 60.0, 500.0, PHYOBJ_STATIC | PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png"))));
    phyContext->addPhyObj(new PhysicsObject(700.0, 600.0, 60.0, 200.0, PHYOBJ_STATIC | PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png"))));
    phyContext->addPhyObj(new PhysicsObject(0.0, 0.0, 900.0, 50.0, PHYOBJ_STATIC | PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png"))));
    phyContext->addPhyObj(new Player(500.0, 920.0, 40.0, 40.0, PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, IMG_Load("tex/Tile.png"))));
    state.startPhysics();

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

        if((*keyState)[SDL_SCANCODE_2] == SDL_KEYDOWN)
        {
            viewport.setZoom(std::min(viewport.getZoom() + 0.01, 2.0));
        }
        else if((*keyState)[SDL_SCANCODE_1] == SDL_KEYDOWN)
        {
            viewport.setZoom(std::max(viewport.getZoom() - 0.01, 0.1));
        }
        
        if((*keyState)[SDL_SCANCODE_UP] == SDL_KEYDOWN)
        {
            SDL_Point newPosition = viewport.getPosition();
            newPosition.y -= 1.0;
            viewport.setPosition(newPosition);
        }
        if((*keyState)[SDL_SCANCODE_DOWN] == SDL_KEYDOWN)
        {
            SDL_Point newPosition = viewport.getPosition();
            newPosition.y += 1.0;
            viewport.setPosition(newPosition);
        }
        if((*keyState)[SDL_SCANCODE_RIGHT] == SDL_KEYDOWN)
        {
            SDL_Point newPosition = viewport.getPosition();
            newPosition.x -= 1.0;
            viewport.setPosition(newPosition);
        }
        if((*keyState)[SDL_SCANCODE_LEFT] == SDL_KEYDOWN)
        {
            SDL_Point newPosition = viewport.getPosition();
            newPosition.x += 1.0;
            viewport.setPosition(newPosition);
        }

        SDL_RenderClear(rend);
        state.draw();
        SDL_RenderPresent(rend);
        SDL_Delay(10);
    }
    state.stopPhysics();
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
 
    return 0;
}