#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <array>

#include "object/image.h"
#include "view.h"

class PhysicsObject;
int defaultPhysLoop(void* data);

class World
{
    public:
        World(SDL_Renderer* rend, View* viewport, SDL_ThreadFunction phyFunction = defaultPhysLoop);
        ~World();
        void draw();
        void update();

        void addImage(Image *newImage);
        void addPhyObj(PhysicsObject* obj);

        //Physics
        void startPhysics();
        void stepPhysics();
        void stopPhysics();
        bool physicsRunning() { return phyRunning; };

        void drawObjects(SDL_Renderer* rend, View viewport);

        double getGravity() { return gravity; };
        void setGravity(double newGravity) { gravity = newGravity; };

        double getPhyTick() { return phyTick; };
        void setPhyTick(double newPhyTick) { phyTick = newPhyTick; };

        std::vector<PhysicsObject*> getphyObjects() { return phyObjects; };
        
    private:
        View* viewport;
        SDL_Renderer* rend;

        std::array<std::vector<Image*>, UINT8_MAX+1> images;
        
        //Physics
        bool phyRunning;
        SDL_ThreadFunction phyFunction;
        SDL_Thread* physThread;

        Uint64 updateTime = 0;
        Uint64 lastRender = 0;
        SDL_mutex* usageLock = SDL_CreateMutex();

        double gravity = 0.00005f;
        double phyTick = 60.0f;
        std::vector<PhysicsObject*> phyObjects;
};

#endif