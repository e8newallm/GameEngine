#include "keystate.h"



KeyState& KeyState::get()
{
    static KeyState singleton;
    return singleton;
}

KeyState::KeyState() :
    keys(SDL_NUM_SCANCODES, SDL_KEYUP)
{
}

void KeyState::update(SDL_Scancode key, SDL_EventType keyEvent)
{
    if(keyEvent == SDL_KEYUP || keyEvent == SDL_KEYDOWN)
        keys[key] = keyEvent;
}

void KeyState::update(SDL_Scancode key, Uint32 keyEvent)
{
    update(key, (SDL_EventType)keyEvent);
}

SDL_EventType KeyState::operator[](SDL_Scancode key)
{
    return keys[key];
}
