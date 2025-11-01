#ifndef OBJECT_H
#define OBJECT_H

#include <SDL3/SDL.h>

#include "graphics.h"
#include "texture_base.h"
#include "world.h"

namespace GameEng
{
/**
 * \brief The base class for all objects that would appear in a world.
 *
 */
class Object
{
  public:
    /**
     * \brief Construct a new object.
     *
     * \param body The dimensions of the object.
     * \param texture The texture of the object.
     */
    Object(SDL_Rect body, std::weak_ptr<Texture_base> texture = NoTexture);

    /**
     * \brief Construct a new object with all dimensions 0 and no assigned texture.
     *
     */
    Object() : Object({0, 0, 0, 0}) {};
    virtual ~Object();

    /**
     * \brief Generates the data required by the image object's shader.
     *
     * \return std::vector<std::byte> The data to be passed to the shader.
     */
    virtual std::vector<std::byte> predraw() = 0;

    /**
     * \brief Draw the object to the given world.
     *
     * \param world The world that the object is being drawn to.
     * \param buffer The buffer that contains the shader data.
     * \param renderPass The renderpass that the object is being drawn under.
     */
    virtual void draw(World* world, SDL_GPUBuffer* buffer, SDL_GPURenderPass* renderPass);

    /**
     * \brief Runs an update on the object.
     *
     * \param deltaTime The time since last update in milliseconds.
     * \param world The world the object is being drawn to.
     */
    virtual void update(double deltaTime, World& world);

    /**
     * \brief Move the object to an absolute position.
     *
     * \param x The new x position of the object.
     * \param y The new y position of the object.
     */
    virtual void move(double x, double y);

    /**
     * \brief Move the object to a relative position.
     *
     * \param x How much to move the object in the x axis.
     * \param y How much to move the object in the y axis.
     */
    virtual void moveDelta(double x, double y);

    /**
     * \brief Resize the object to an absolute size.
     *
     * \param height The new height of the object.
     * \param width The new width of the object.
     */
    virtual void resize(double height, double width);

    /**
     * \brief Resize the object to a relative size.
     *
     * \param height How much to adjust the height of the object by.
     * \param width How much to adjust the width of the object by.
     */
    virtual void resizeDelta(double height, double width);

    /**
     * \brief Get the body of the object.
     *
     * \return const SDL_Rect* the object's body.
     */
    [[nodiscard]] virtual const SDL_Rect* getBody() const
    {
        return &body;
    };

  protected:
    SDL_Rect body;
    std::shared_ptr<Texture_base> tex;
};
} // namespace GameEng

#endif
