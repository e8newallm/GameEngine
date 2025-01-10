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

class TimerLoop : public Timer<>
{
    public:
        TimerLoop(double Hz) : Timer<>()
        ,Hz(Hz)
        {};

        bool loopTrigger()
        {
            if(getElapsed() >= 1000.0f / Hz)
            {
                update();
                return true;
            }
            return false;
        };

    private:
        double Hz;
};

#endif