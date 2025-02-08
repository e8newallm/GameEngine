#ifndef WORLD_H
#define WORLD_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <mutex>

#include "timer.h"
#include "view.h"

class Object;

class World
{
    public:
        World(SDL_GPUDevice* gpu, View viewport);
        ~World();
        void draw(double deltaTime, SDL_Window* win);
        void update();
        void addObj(Object* obj);

        //Physics
        void startPhysics();
        void stopPhysics();
        void runPhysics();
        
        bool physicsRunning() const { return phyRunning; };

        double getGravity() const { return gravity; };
        void setGravity(double newGravity) { gravity = newGravity; };

        const std::vector<Object*>& getObjects() const { return objects; };

        View& getView();
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

        Timer<> lastPhysics;
        std::mutex usageLock;
        double gravity = 0.00005f;
        const double pps = 60.0f;
        std::vector<Object*> objects;
};

#endif