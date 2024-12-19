#ifndef KEYSTATE_H
#define KEYSTATE_H

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_events.h>

#include <array>

class KeyState
{
    public:
        static void update();
        static void updateKey(SDL_Scancode key, SDL_EventType keyEvent);
        static void updateKey(SDL_Scancode key, Uint32 keyEvent);
        static SDL_EventType key(SDL_Scancode key);

    private:
        static inline std::array<SDL_EventType, SDL_NUM_SCANCODES> keys;
        KeyState(const KeyState&) = delete;
};
#endif

/*keys(SDL_NUM_SCANCODES, SDL_KEYUP)*/