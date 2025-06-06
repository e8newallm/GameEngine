#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include "object.h"

namespace GameEng
{
class Context;

/**
 * \brief An Object for drawing images to the world.
 *
 */
class Image : public Object
{
  public:
    /**
     * \brief Construct a new Image object.
     *
     * \param body The body rectangle of the image.
     * \param texture The image to be drawn.
     */
    Image(SDL_Rect body, const std::string& texture);

    /**
     * \brief Generates the data required by the image object's shader.
     *
     * \return std::vector<std::byte> The data to be passed to the shader.
     */
    std::vector<std::byte> predraw() override;
};
} // namespace GameEng

#endif