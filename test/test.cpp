#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_thread.h>

#include <iostream>
#include <ctime>

#include "object.h"
#include "physicsobject.h"
#include "context.h"

#include "mousestate.h"
#include "keystate.h"

#include "catch_all.hpp"

TEST_CASE( "Basic functionality", "[function]" ) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(nullptr, -1, render_flags);
    KeyState* keyState = KeyState::get();
    MouseState* mouseState = MouseState::get();

    View viewport( {1000, 1000}, {0, 0});
    viewport.setZoom(1.0);

    Context state(rend, &viewport);

    PhysicsContext* phyContext = state.getPhysicsContext();
    PhysicsObject* Box = new PhysicsObject(500.0, 500.0, 10.0, 10.0, PHYOBJ_COLLIDE, SDL_CreateTextureFromSurface(rend, nullptr));
    phyContext->addPhyObj(Box);

    SECTION( "Box drop" ) {
        Box->velocityDelta(0.0, 0.5);
        Box->preUpdate();
        Box->update(10.0, phyContext);
        REQUIRE(Box->getBody()->x == 500);
        REQUIRE(Box->getBody()->y == 505);
        Box->preUpdate();
        Box->update(2.0, phyContext);
        REQUIRE(Box->getBody()->x == 500);
        REQUIRE(Box->getBody()->y == 506);

        Box->preUpdate();
        Box->update(1.0, phyContext);
        REQUIRE(Box->getBody()->x == 500);
        REQUIRE(Box->getBody()->y == 506);
        Box->preUpdate();
        Box->update(1.0, phyContext);
        REQUIRE(Box->getBody()->x == 500);
        REQUIRE(Box->getBody()->y == 507);

        Box->velocityDelta(15.0, 0.0);
        Box->preUpdate();
        Box->update(4.0, phyContext);
        REQUIRE(Box->getBody()->x == 559);
        REQUIRE(Box->getBody()->y == 509);

        Box->moveDelta(10.0, 10.0);
        Box->preUpdate();
        Box->update(45.0, phyContext);
        REQUIRE(Box->getBody()->x == 1244);
        REQUIRE(Box->getBody()->y == 541);
    }
}