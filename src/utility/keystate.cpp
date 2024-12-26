#include <SDL_events.h>
#include <iostream>

#include "keystate.h"
#include "logging.h"

 std::array<SDL_EventType, SDL_NUM_SCANCODES> keyStateInit()
 {
    std::array<SDL_EventType, SDL_NUM_SCANCODES> init;
    for(int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        init[i] = SDL_KEYUP;
    }
    return init;
 }

void KeyState::update()
{
    keysPrev = keys;
    while(1)
    {
        SDL_Event event;
        SDL_PumpEvents();
        int count = SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_KEYDOWN, SDL_KEYUP);
        if(count <= 0) break;

        switch (event.type)
        {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                updateKey(event.key.keysym.scancode, event.type);
                break;
            }
            default:
            {
                Logger::warning("Unhandled keyboard event: " + std::format("{:x}", event.type));
            }
        }
    }
}

void KeyState::updateKey(SDL_Scancode key, SDL_EventType keyEvent)
{
    if(keyEvent == SDL_KEYUP || keyEvent == SDL_KEYDOWN)
        keys[key] = keyEvent;
}

void KeyState::updateKey(SDL_Scancode key, Uint32 keyEvent)
{
    updateKey(key, (SDL_EventType)keyEvent);
}

SDL_EventType KeyState::key(SDL_Scancode key)
{
    return keys[key];
}

bool KeyState::keyPressed(SDL_Scancode key)
{
    return keys[key] == SDL_KEYDOWN && keysPrev[key] == SDL_KEYUP;
}

bool KeyState::keyReleased(SDL_Scancode key)
{
    return keys[key] == SDL_KEYUP && keysPrev[key] == SDL_KEYDOWN;
}