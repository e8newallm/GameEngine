#ifndef VIEW_H
#define VIEW_H

#include <SDL2/SDL_rect.h>

class View
{
    public:
        View(SDL_Point resolution, SDL_Point position) :
            resolution(resolution)
            , position(position)
            , xZoomOffset (resolution.x / 2 * (1.0 - zoom))
            , yZoomOffset (resolution.y / 2 * (1.0 - zoom))
        {};

        double getZoom() { return zoom; };
        double getZoomXOffset() { return xZoomOffset; };
        double getZoomYOffset() { return yZoomOffset; };
        void setZoom(double zoom)
        {
            this->zoom = zoom;
            xZoomOffset = resolution.x / 2 * (1.0 - getZoom());
            yZoomOffset = resolution.y / 2 * (1.0 - getZoom());
        };

        SDL_Point getResolution() { return resolution; };
        void setResolution(SDL_Point resolution) { this->resolution = resolution; };

        SDL_Point getPosition() { return position; };
        void setPosition(SDL_Point position) { this->position = position; };

    private:

        double zoom = 1.0;
        double xZoomOffset, yZoomOffset;
        SDL_Point resolution;
        SDL_Point position;
};
#endif