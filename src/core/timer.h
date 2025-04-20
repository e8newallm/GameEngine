#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <ratio>

#define MILLISECONDS 1000.0

namespace GameEng
{
    /**
     * \brief A class for keeping track of time and triggering code at a fixed rate.
     *
     */
    class Timer
    {
        public:
            /**
             * \brief Construct a new Timer object.
             *
             * \param rate The frequency at which the triggering should occur.
             */
            explicit Timer(double rate) : timeframe(MILLISECONDS/(rate)), nextTrigger(timeframe) {};

            /**
             * \brief Get the elapsed time since the last trigger.
             * 
             * \return double The elapsed time in milliseconds.
             */
            [[nodiscard]] double getElapsed() const { return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - timer).count(); };

            /**
             * \brief Checks if the timer is ready to trigger.
             * 
             * \return bool Is the timer ready to trigger?
             */
             [[nodiscard]] bool trigger() const { return getElapsed() >= nextTrigger; };

            /**
             * \brief Updates the counter with the next trigger timing.
             * 
             */
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