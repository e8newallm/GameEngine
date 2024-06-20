#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <array>

#include "object/physicsobject.h"
#include "object/object.h"
#include "object/image.h"
#include "keystate.h"
#include "view.h"

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}

class Context
{
    public:
        Context(SDL_Renderer* rend, View* viewport, SDL_ThreadFunction phyFunction = nullptr);
        void draw();

        PhysicsContext* getPhysicsContext() { return phyContext; };
        void startPhysics();
        void stepPhysics();
        void stopPhysics();
        bool physicsRunning() { return phyRunning; };

        void addImage(Image* newImage);
        
        void runLuaFile(std::string filename);
        void runLuaStr(std::string luaScript);
    private:
        View* viewport;
        SDL_Renderer* rend;
        KeyState& keyState;
        PhysicsContext* phyContext;
        bool phyRunning;
        SDL_ThreadFunction phyFunction;
        SDL_Thread* physThread;
        lua_State* luaState;

        std::array<std::vector<Image*>, UINT8_MAX+1> images;
};

#endif