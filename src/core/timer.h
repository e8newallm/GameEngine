#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <ratio>

namespace GameEng
{
    class Timer
    {
        public:
            explicit Timer(double rate) : timeframe(1000.0/(rate)), nextTrigger(timeframe) {};

            double getElapsed() const { return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - timer).count(); };
            bool trigger() const { return getElapsed() >= nextTrigger; };
            void update()
            {
                double elapsed = getElapsed();
                nextTrigger = timeframe - (elapsed - timeframe);
                timer = std::chrono::high_resolution_clock::now();
            };

        private:
            double timeframe;
            double nextTrigger;
            std::chrono::high_resolution_clock::time_point timer = std::chrono::high_resolution_clock::now();
    };
}

#endif