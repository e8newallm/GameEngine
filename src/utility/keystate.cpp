#include <SDL3/SDL.h>

#include "keystate.h"
#include "logging.h"

namespace GameEng
{
    std::array<SDL_EventType, SDL_SCANCODE_COUNT> keyStateInit()
    {
        std::array<SDL_EventType, SDL_SCANCODE_COUNT> init;
        for(int i = 0; i < SDL_SCANCODE_COUNT; i++)
        {
            init[i] = SDL_EVENT_KEY_UP;
        }
        return init;
    }

    void KeyState::update()
    {
        keysPrev = keys;
        SDL_PumpEvents();
        while(1)
        {
            SDL_Event event;
            int count = SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENT_KEY_DOWN, SDL_EVENT_KEY_UP);
            if(count <= 0) break;

            switch (event.type)
            {
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP:
                {
                    updateKey(event.key.scancode, event.type);
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
        if(keyEvent == SDL_EVENT_KEY_UP || keyEvent == SDL_EVENT_KEY_DOWN)
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
        return keys[key] == SDL_EVENT_KEY_DOWN && keysPrev[key] == SDL_EVENT_KEY_UP;
    }

    bool KeyState::keyReleased(SDL_Scancode key)
    {
        return keys[key] == SDL_EVENT_KEY_UP && keysPrev[key] == SDL_EVENT_KEY_DOWN;
    }
}