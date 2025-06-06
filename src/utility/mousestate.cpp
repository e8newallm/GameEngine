#include <SDL3/SDL.h>

#include "logging.h"
#include "mousestate.h"

namespace GameEng
{
void MouseState::reset()
{
    for(int i = 1; i <= SDL_BUTTON_X2; i++)
    {
        mouseButton[i] = SDL_MouseButtonEvent();
    }
    xDeltaMouse = 0;
    yDeltaMouse = 0;
    scrollAmount = 0;
}

void MouseState::update()
{
    reset();
    SDL_PumpEvents();
    while(true)
    {
        SDL_Event event;
        int count = SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENT_MOUSE_MOTION, SDL_EVENT_MOUSE_WHEEL);
        if(count <= 0)
        {
            break;
        }

        switch(event.type)
        {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            updateButton(event.button);
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            updateMove(event.motion);
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: {
            updateWheel(event.wheel);
            break;
        }
        default: {
            Logger::warning("Unhandled mouse event: ", std::format("{:x}", event.type));
        }
        }
    }
}

void MouseState::updateButton(SDL_MouseButtonEvent buttonPress)
{
    mouseButton[buttonPress.button] = buttonPress;
    mouseButtonDown[buttonPress.button] = (buttonPress.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
}

void MouseState::updateMove(SDL_MouseMotionEvent mouseMove)
{
    xMouse = mouseMove.x;
    yMouse = mouseMove.y;
    xDeltaMouse += mouseMove.xrel;
    yDeltaMouse += mouseMove.yrel;
}

void MouseState::updateWheel(SDL_MouseWheelEvent wheelScroll)
{
    scrollAmount -= (wheelScroll.direction == SDL_MOUSEWHEEL_FLIPPED) ? -static_cast<int>(wheelScroll.y)
                                                                      : static_cast<int>(wheelScroll.y);
}
} // namespace GameEng