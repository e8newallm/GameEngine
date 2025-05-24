#ifndef WORLD_H
#define WORLD_H

#include <SDL3/SDL.h>
#include <mutex>

#include "timer.h"
#include "view.h"

namespace GameEng
{
    class Object;
    class World;


    using GEUpdateFunc = void (*)(double deltaTime, World& world);

    /**
     * \brief A class for defining a given world for objects to exist and interact within.
     * 
     */
    class World
    {
        public:
            /**
            * \brief Construct a new world.
            *
            * \param gpu The SDL_GPUDevice to draw with.
            * \param viewport The initial view of the world.
            */
            World(SDL_GPUDevice* gpu, View viewport);
            ~World();

            /**
             * \brief Draw the world to the given window.
             * 
             * \param win The window to draw to.
             */
            void draw(SDL_Window* win);

            /**
             * \brief Runs the registered World update function (if one is registered) and updates the objects in the world.
             *
             * \param deltaTime The time since last update in milliseconds.
             */
            void update(double deltaTime);

            /**
             * \brief Registers a function to run when the world is updated (useful for things such as checking keyboard/mouse inputs).
             * 
             * \param func The function to be registered.
             */
            void registerUpdate(GEUpdateFunc func);

            /**
             * \brief Adds an object to the world.
             *
             * \param obj The object to be added to the world.
             */
            void addObj(Object* obj);

            /**
             * \brief Gets a list of current objects in the world.
             * 
             * \return const std::vector<Object*>& The list of objects in the world.
             */
            [[nodiscard]] const std::vector<Object*>& getObjects() const { return objects; };

            //Physics

            /**
             * \brief Starts the physics of this world running.
             *
             */
            void startPhysics();

            /**
            * \brief Pauses the physics of the world running.
            *
            */
            void stopPhysics();

            /**
             * \brief Runs one physics update tick if it is time for a new update tick (As long as the physics isn't paused and the game isn't paused).
             *
             * \return bool Was it time for a new update tick?
             */
            bool runPhysics();

            /**
             * \brief Returns if the physics is currently running or paused.
             * 
             * \return bool Is the physics running?
             */
            [[nodiscard]] bool physicsRunning() const { return phyRunning; };

            /**
             * \brief Returns the physics update frequency.
             *
             * \return double The physics update frequency.
             */
            [[nodiscard]] double getPPS() const { return pps; };

            /**
             * \brief Sets the current physics interpolation to a new value (Used for unit testing).
             *
             * \param newPhyInter The new physics interpolation value.
             */
            void setPhyInterpolation(double newPhyInter) { phyInterPercent = newPhyInter; };

            /**
             * \brief Get the physics interpolation value (For frame updates inbetween physics ticks).
             *
             * \return double The current physics interpolation value.
             */
            [[nodiscard]] double getPhyInterpolation() const { return phyInterPercent; };

            /**
             * \brief Gets the current gravity value.
             *
             * \return double The current gravity.
             */
            [[nodiscard]] double getGravity() const { return gravity; };

            /**
             * \brief Sets the gravity.
             *
             * \param newGravity New gravity value.
             */
            void setGravity(double newGravity) { gravity = newGravity; };

            /**
             * \brief Gets the view of the world.
             *
             * \return View& The current world view.
             */
            View& getView();

            /**
             * \brief Gets the current GPU device.
             * 
             * \return SDL_GPUDevice* The current GPU device.
             */
            SDL_GPUDevice* getGPU();

        protected:
            struct ShaderWorldData
            {
                SDL_Rect camera;
            };

        private:
            View viewport;
            SDL_GPUDevice* gpu;

            //Physics
            bool phyRunning;
            double phyInterPercent = 0.0F;
            const double pps = 60.0F;
            Timer physicsTimer;

            GEUpdateFunc updateFunc = nullptr;

            const double initialGravity =  0.00005F;
            double gravity = initialGravity;

            std::mutex usageLock;
            std::vector<Object*> objects;
    };
}

#endif