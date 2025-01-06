#ifndef WORLD_H
#define WORLD_H

#include <SDL2/SDL.h>
#include <vector>
#include <array>
#include <chrono>

#include "view.h"

class Image;
class PhysicsObject;
int defaultPhysLoop(void* data);

using timer = std::chrono::high_resolution_clock::time_point;

class World
{
    public:
        World(SDL_Renderer* rend, View viewport);
        ~World();
        void draw(timer lastRender);
        void update();
        void addImage(Image* newImage);
        void addPhyObj(PhysicsObject* obj);

        //Physics
        void startPhysics();
        void stopPhysics();
        void runPhysics();
        
        bool physicsRunning() { return phyRunning; };

        double getGravity() { return gravity; };
        void setGravity(double newGravity) { gravity = newGravity; };

        std::vector<PhysicsObject*> getphyObjects() { return phyObjects; };

        View& getViewpoint() { return viewport; };
        SDL_Renderer* getRend() { return rend; };
        
    private:
        View viewport;
        SDL_Renderer* rend;

        std::array<std::vector<Image*>, UINT8_MAX+1> images;
        
        //Physics
        bool phyRunning;
        SDL_Thread* physThread;

        timer lastPhysics;
        SDL_mutex* usageLock = SDL_CreateMutex();
        double gravity = 0.00005f;
        const double pps = 60.0f;
        std::vector<PhysicsObject*> phyObjects;
};

#endif