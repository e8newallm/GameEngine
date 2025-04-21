#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "object.h"
#include "texture_base.h"
#include "world.h"

enum PhyObjFlag : int //NOLINT(performance-enum-size)
{
    Static  = 1<<0,
    Collide = 1<<1,
};

namespace GameEng
{
    /**
     * \brief Base class for all objects that would have physics applied to them.
     * 
     */
    class PhysicsObject : public Object
    {
        public:
            /**
             * \brief Construct a new physics object.
             *
             * \param body The dimensions of the physics object.
             * \param flags Various flags that affect how the physics applies.
             * \param texture The texture of the physics object.
             */
            PhysicsObject(SDL_Rect body, int flags, std::shared_ptr<Texture_base> texture = NoTexture);

            /**
             * \brief Construct a new physics object with all dimensions 0, no PhyObj flags, and no assigned texture.
             *
             */
            PhysicsObject() = default;

            using Object::draw;
            using Object::update;

            /**
             * \brief Generates the data required by the image object's shader.
             *
             * \return ShaderObjData The data to be passed to the shader.
             */
            ShaderObjData predraw() override;

            /**
             * \brief Runs an update on the physics object.
             *
             * \param deltaTime The time since last update in milliseconds.
             * \param world The world the object is being drawn to.
             */
            void update(double deltaTime, World& world) override;

            /**
             * \brief Runs one physics update tick for the object.
             * 
             * \param deltaTime The time since last update in milliseconds.
             * \param world The world that the physics object exists in.
             */
            virtual void runPhysics(double deltaTime, World& world);

            /**
             * \brief Checks whether the physics object is currently on top of another collidable physics object.
             * 
             * \param world The world that the physics object exists in.
             * \return bool Is the physics object on top of another physics object.
             */
            [[nodiscard]] bool onGround(const World& world) const;

            /**
             * \brief Sets the velocity of the physics object to an absolute value.
             *
             * \param x The new x velocity of the physics object.
             * \param y The new y velocity of the physics object.
             */
            virtual void velocity(double x, double y);

            /**
             * \brief Adjusts the velocity of the physics object to a relative value.
             *
             * \param x The value to adjust the x velocity by.
             * \param y The value to adjust the y velocity by.
             */
            virtual void velocityDelta(double x, double y);

            /**
             * \brief Get the velocity of the physics object.
             *
             * \return SDL_FPoint The velocity of the physics object.
             */
            [[nodiscard]] virtual SDL_FPoint getVelocity() { return nextVelocity; };

            /**
             * \brief Get the body of the physics object.
             *
             * \return const SDL_Rect* the physics object's body.
             */
            [[nodiscard]] const SDL_Rect* getBody() const override { return &interBody; };

            /**
             * \brief Returns if the physics object is static.
             *
             * \return bool Is the object static?
             */
            [[nodiscard]] bool isStatic() const { return _isStatic; };

            /**
             * \brief Returns if the physics object is collidable.
             *
             * \return bool Is the object collidable?
             */
            [[nodiscard]] bool canCollide() const { return _canCollide; };

        protected:
            bool _isStatic = true;
            bool _canCollide = false;

            SDL_Rect nextBody;
            SDL_Rect interBody;

            SDL_FPoint currentVelocity;
            SDL_FPoint nextVelocity;
    };
}

#endif