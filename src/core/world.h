#ifndef WORLD_H
#define WORLD_H

#include <SDL3/SDL.h>
#include <mutex>

#include "timer.h"
#include "view.h"

class Object;
class World;

typedef void (*GEUpdateFunc)(double deltaTime, World& world);

class World
{
    public:
        World(SDL_GPUDevice* gpu, View viewport);
        ~World();
        void draw(double deltaTime, SDL_Window* win);
        void update(double deltaTime);

        void addObj(Object* obj);
        const std::vector<Object*>& getObjects() const { return objects; };

        //Physics
        void startPhysics();
        void stopPhysics();
        void runPhysics();
        bool physicsRunning() const { return phyRunning; };
        bool getPPS() const { return pps; };

        void setPhyInterpolation(double newPhyInter) { phyInterPercent = newPhyInter; };
        double getPhyInterpolation() const { return phyInterPercent; };

        double getGravity() const { return gravity; };
        void setGravity(double newGravity) { gravity = newGravity; };

        View& getView();
        SDL_GPUDevice* getGPU();

        void registerUpdate(GEUpdateFunc func);

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
        double phyInterPercent = 0.0f;
        const double pps = 60.0f;

        GEUpdateFunc updateFunc = nullptr;

        std::mutex usageLock;
        double gravity = 0.00005f;
        std::vector<Object*> objects;
};

#endif