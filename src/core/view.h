#ifndef VIEW_H
#define VIEW_H

#include <SDL3/SDL.h>
#include <string>

#include "logging.h"

namespace GameEng
{
    class View
    {
        public:
            View(SDL_FPoint resolution, SDL_FPoint position) :
            zoom(1.0f)
            ,resolution(resolution)
            ,center(position)
            {
                calcWindow();
            };

            double getZoom() const { return zoom; };
            void setZoom(double zoom)
            {
                this->zoom = zoom;
                calcWindow();
            };

            void move(SDL_Point newPosition)
            {
                center.x = newPosition.x;
                center.y = newPosition.y;
                calcWindow();
            }

            void moveDelta(SDL_FPoint newPosition)
            {
                center.x -= newPosition.x/zoom;
                center.y -= newPosition.y/zoom;
                calcWindow();
            }

            SDL_Rect* window()
            {
                return &win;
            };

        private:

            SDL_Rect calcWindow()
            {
                win.w = resolution.x/zoom;
                win.h = resolution.y/zoom;
                win.x = center.x - resolution.x/zoom/2.0f;
                win.y = center.y - resolution.y/zoom/2.0f;
                Logger::debug("window: " + std::to_string(win.x) + ", " + std::to_string(win.y) + " - " + std::to_string(win.w) + ", " + std::to_string(win.h));
                return win;
            }

            double zoom;
            SDL_FPoint resolution;
            SDL_FPoint center;
            SDL_Rect win;
    };
}

#endif