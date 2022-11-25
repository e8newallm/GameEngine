#ifndef MOUSESTATE_H
#define MOUSESTATE_H

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_events.h>

#include <vector>
#include <array>

struct mousePosition
{
    Uint32 x;
    Uint32 y;
};

class MouseState
{
    public:
        static MouseState* get();

        void updateButton(SDL_MouseButtonEvent buttonPress);
        void updateMove(SDL_MouseMotionEvent mouseMove);
        void updateWheel(SDL_MouseWheelEvent wheelScroll);
        void reset();
        SDL_EventType operator[](Uint8 button);

        mousePosition mousePos() 
            { return {xMouse, yMouse}; };
        
        SDL_Point mouseDelta() 
            { return {xDeltaMouse, yDeltaMouse}; };

        Sint32 scrollDelta() 
            { return scrollAmount; };

        bool buttonDown(Uint8 button) 
            { return mouseButtonDown[button]; };

        bool doubleClicked(Uint8 button)
            { return mouseButton[button].clicks == 2; };

        bool clicked(Uint8 button) 
            { return mouseButton[button].clicks == 1; };

        SDL_Point clickPosition(Uint8 button) 
            { return {mouseButton[button].x, mouseButton[button].y}; };

    private:
        Uint32 xMouse, yMouse;
        Sint32 xDeltaMouse, yDeltaMouse;
        std::array<bool, SDL_BUTTON_X2+1> mouseButtonDown;
        std::array<SDL_MouseButtonEvent, SDL_BUTTON_X2+1> mouseButton;
        Sint32 scrollAmount;
        MouseState();
        static MouseState* singleton;
};

#endif