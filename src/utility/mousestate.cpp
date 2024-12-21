#include <SDL_events.h>

#include "mousestate.h"
#include "logging.h"

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
    while(1)
    {
        SDL_Event event;
        SDL_PumpEvents();
        int count = SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_MOUSEMOTION, SDL_MOUSEWHEEL);
        if(count <= 0) break;

        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                updateButton(event.button);
                break;
            }
            case SDL_MOUSEMOTION:
            {
                updateMove(event.motion);
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                updateWheel(event.wheel);
                break;
            }
            default:
            {
                Logger::warning("Unhandled mouse event: " + std::format("{:x}", event.type));
            }
        }
    }
}

void MouseState::updateButton(SDL_MouseButtonEvent buttonPress)
{
    mouseButton[buttonPress.button] = buttonPress;
    mouseButtonDown[buttonPress.button] = (buttonPress.type == SDL_MOUSEBUTTONDOWN);
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
    scrollAmount += (wheelScroll.direction == SDL_MOUSEWHEEL_FLIPPED) ? -wheelScroll.y : wheelScroll.y;
}