#ifndef VIEW_H
#define VIEW_H

#include <SDL3/SDL.h>
#include <iostream>

class View
{
    public:
        View(SDL_Point resolution, SDL_Point position) :
        zoom(1.0f)
        ,center{(position.x + resolution.x)/2, (position.y + resolution.y)/2}
        ,resolution(resolution)
        {
            calcWindow();
        };

        double getZoom() { return zoom; };
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

        void moveDelta(SDL_Point newPosition)
        {
            center.x += newPosition.x/zoom;
            center.y += newPosition.y/zoom;
            calcWindow();
        }

        SDL_Rect* window()
        {
            return &win;
        };

    private:

        SDL_Rect calcWindow()
        {
            //std::cout << "center:" << center.x << ", " << center.y << " resolution: " << resolution.x << ", " << resolution.y << " zoom: " << zoom << "\r\n";
            win.w = resolution.x/zoom;
            win.h = resolution.y/zoom;
            win.x = center.x - resolution.x/zoom/2.0f;
            win.y = center.y - resolution.x/zoom/2.0f;
            return win;
        }

        double zoom;
        SDL_Point center;
        SDL_Point resolution;

        SDL_Rect win;
};
#endif