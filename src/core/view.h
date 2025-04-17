#ifndef VIEW_H
#define VIEW_H

#include <SDL3/SDL.h>
#include <string>

#include "logging.h"

namespace GameEng
{
    /**
     * \brief A class for representing the camera of a world.
     * 
     */
    class View
    {
        public:

            /**
             * \brief Construct a new View object.
             * 
             * \param resolution The resolution of the view.
             * \param position The initial position of the view inside a given world.
             */
            View(SDL_FPoint resolution, SDL_FPoint position) :
            zoom(1.0f)
            ,resolution(resolution)
            ,center(position)
            {
                calcWindow();
            };

            /**
             * \brief Get the current zoom of the view.
             * 
             * \return double The zoom of the view.
             */
            double getZoom() const { return zoom; };

            /**
             * \brief Set the zoom of the view.
             * 
             * \param zoom The new zoom of the view.
             */
            void setZoom(double zoom)
            {
                this->zoom = zoom;
                calcWindow();
            };

            /**
             * \brief Move the view to a absolute position.
             *
             * \param newPosition The new position of the view.
             */
            void move(SDL_Point newPosition)
            {
                center.x = newPosition.x;
                center.y = newPosition.y;
                calcWindow();
            }

            /**
             * \brief Move the view to a relative position.
             *
             * \param delta The amount to move the view by.
             */
            void moveDelta(SDL_FPoint delta)
            {
                center.x -= delta.x/zoom;
                center.y -= delta.y/zoom;
                calcWindow();
            }

            /**
             * \brief Return the current view rectangle.
             *
             * \return SDL_Rect* The current rectangle of the view.
             */
            SDL_Rect* window()
            {
                return &win;
            };

        private:

            /**
             * \brief Calculate the new view rectangle after zoom/positional adjustments.
             *
             */
            void calcWindow()
            {
                win.w = resolution.x/zoom;
                win.h = resolution.y/zoom;
                win.x = center.x - resolution.x/zoom/2.0f;
                win.y = center.y - resolution.y/zoom/2.0f;
                Logger::debug("window: " + std::to_string(win.x) + ", " + std::to_string(win.y) + " - " + std::to_string(win.w) + ", " + std::to_string(win.h));
            }

            double zoom;
            SDL_FPoint resolution;
            SDL_FPoint center;
            SDL_Rect win;
    };
}

#endif