#include "mousestate.h"

MouseState* MouseState::singleton = nullptr;

MouseState* MouseState::get()
{
    if (singleton == nullptr)
        singleton = new MouseState();
    
    return singleton;
}

MouseState::MouseState() :
    xMouse(0)
    , yMouse(0)
    , xDeltaMouse(0)
    , yDeltaMouse(0)
    , scrollAmount(0)
    , mouseButtonDown()
    , mouseButton()
{
}

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