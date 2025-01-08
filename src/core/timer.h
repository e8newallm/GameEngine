#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <ratio>

template <class rep = double, class period = std::milli> class Timer
{
    public:
        rep getElapsed() const { return std::chrono::duration<rep, period>(std::chrono::high_resolution_clock::now() - timer).count(); };
        void update() { timer = std::chrono::high_resolution_clock::now(); };

    private:
        std::chrono::high_resolution_clock::time_point timer = std::chrono::high_resolution_clock::now();
};

#endif