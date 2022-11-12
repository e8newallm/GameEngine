#ifndef KEYSTATE_H
#define KEYSTATE_H

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_events.h>

#include <vector>

class KeyState
{
    public:
        static KeyState* get();

        void update(SDL_Scancode key, SDL_EventType keyEvent);
        void update(SDL_Scancode key, Uint32 keyEvent);
        SDL_EventType operator[](SDL_Scancode key);

    private:
        std::vector<SDL_EventType> keys;
        KeyState();
        static KeyState* singleton;
};

#endif