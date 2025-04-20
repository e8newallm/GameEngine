#ifndef MOUSESTATE_H
#define MOUSESTATE_H

#include <SDL3/SDL.h>

#include <array>

namespace GameEng
{
     /**
     * \brief A static class for keeping track of the mouse state.
     *
     */
    class MouseState
    {
        public:
            /**
             * \brief Resets the MouseState before updating to the current state.
             * 
             */
            static void reset();

            /**
             * \brief Peeps and processes all SDL keyboard events.
             * Should be used at the start of each frame.
             */
            static void update();

            /**
             * \brief Updates the state of a button to a given event.
             *
             * \param buttonPress The event that updates a button.
             */
            static void updateButton(SDL_MouseButtonEvent buttonPress);

            /**
             * \brief Updates the position of the mouse through an event.
             *
             * \param mouseMove The mouse motion event to update with.
             */
            static void updateMove(SDL_MouseMotionEvent mouseMove);

            /**
             * \brief Updates the mouse wheel state through an event.
             * 
             * \param wheelScroll The mouse wheel event to update with.
             */
            static void updateWheel(SDL_MouseWheelEvent wheelScroll);

            /**
             * \brief Get the mouse position in the window.
             *
             * \return SDL_FPoint The mouse position in the window.
             */
            static SDL_FPoint mousePos()
                { return {xMouse, yMouse}; };

            /**
             * \brief Get the mouse position relative to the last update in the window.
             *
             * \return SDL_FPoint The mouse position delta since the last update in the window.
             */
            static SDL_FPoint mouseDelta()
                { return {xDeltaMouse, yDeltaMouse}; };

            /**
             * \brief Get the mouse wheel scroll distance relative to the last update.
             *
             * \return Sint32 Get the mouse wheel scroll delta.
             */
            static Sint32 scrollDelta()
                { return scrollAmount; };

            /**
             * \brief Checks if a button is currently pressed down.
             *
             * \param button The button being checked.
             * \return bool Is the button currently pressed down?
             */
            static bool buttonDown(Uint8 button)
                { return mouseButtonDown[button]; };

            /**
             * \brief Checks if the button has been double clicked.
             *
             * \param button The button being checked.
             * \return bool Has the button been double clicked?
             */
            static bool doubleClicked(Uint8 button)
                { return mouseButton[button].clicks == 2; };

            /**
             * \brief Checks if the button has been clicked.
             *
             * \param button The button being checked.
             * \return bool Has the button been clicked?
             */
            static bool clicked(Uint8 button)
                { return mouseButton[button].clicks == 1; };

            /**
             * \brief Gets the position that a given button was pressed in the window.
             *
             * \param button The button whose click position is being gotten.
             * \return SDL_Point The position in the window the click happened.
             */
            static SDL_Point clickPosition(Uint8 button)
                { return {(int)mouseButton[button].x, (int)mouseButton[button].y}; };

            MouseState() = delete;
            MouseState(const MouseState&) = delete;
            MouseState& operator=(const MouseState&) = delete;

        private:
            static inline float xMouse = 0, yMouse = 0;
            static inline float xDeltaMouse = 0, yDeltaMouse = 0;
            static inline Sint32 scrollAmount = 0;
            static inline std::array<bool, SDL_BUTTON_X2+1> mouseButtonDown;
            static inline std::array<SDL_MouseButtonEvent, SDL_BUTTON_X2+1> mouseButton;
    };
}

#endif