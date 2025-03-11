#ifndef KEYSTATE_H
#define KEYSTATE_H

#include <SDL3/SDL.h>

#include <array>

namespace GameEng
{
    std::array<SDL_EventType, SDL_SCANCODE_COUNT> keyStateInit();

    class KeyState
    {
        public:
            static void update();
            static void updateKey(SDL_Scancode key, SDL_EventType keyEvent);
            static void updateKey(SDL_Scancode key, Uint32 keyEvent);
            static SDL_EventType key(SDL_Scancode key);

            static bool keyPressed(SDL_Scancode key);
            static bool keyReleased(SDL_Scancode key);
            
        private:
            static inline std::array<SDL_EventType, SDL_SCANCODE_COUNT> keys = keyStateInit();
            static inline std::array<SDL_EventType, SDL_SCANCODE_COUNT> keysPrev = keyStateInit();

            KeyState() = delete;
            KeyState(const KeyState&) = delete;
            KeyState& operator=(const KeyState&) = delete;
    };
}

#endif