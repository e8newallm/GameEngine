#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <source_location>
#include <filesystem>

namespace GameEng
{
    /**
     * \brief A class for handling logging of the game.
     * 
     */
    class Logger
    {
        public:
            /**
             * \brief Logs an error in the log.
             *
             * \param msg The message being logged.
             * \param location The location in source where the log message was used (Does not show in a release build)
             */
            static void error(const std::string& msg, const std::source_location location = std::source_location::current());

            /**
             * \brief Logs an warning in the log.
             *
             * \param msg The message being logged.
             * \param location The location in source where the log message was used (Does not show in a release build)
             */
            static void warning(const std::string& msg, const std::source_location location = std::source_location::current());

            /**
             * \brief Logs an message in the log.
             *
             * \param msg The message being logged.
             * \param location The location in source where the log message was used (Does not show in a release build)
             */
            static void message(const std::string& msg, const std::source_location location = std::source_location::current());

            /**
             * \brief Logs an debug in the log.
             * Does not show in a release build
             * \param msg The message being logged.
             * \param location The location in source where the log message was used (Does not show in a release build)
             */
            static void debug(const std::string& msg, const std::source_location location = std::source_location::current());

            /**
             * \brief Initialises the log to output to a given ostream, replacing std::cout.
             * Init also registers deinit function to run on game close.
             * \param output The ostream to write log messages to instead of std::cout.
             */
            static void init(std::ostream* output);

            /**
             * \brief Initialises the log to output to a given file location, replacing std::cout.
             * Init also registers deinit function to run on game close.
             * \param output The file location to write log messages to instead of std::cout.
             */
            static void init(std::filesystem::path fileLocation);

            /**
             * \brief Deinitialises the logger to close a file being logged to and resets the ostream to std::cout.
             * 
             */
            static void deinit();

        private:
            static void log(const std::string& msg,
            const std::source_location location =
                std::source_location::current());

            static inline std::ostream* logFile = &std::cout;
            static inline std::ofstream* file = nullptr;
    };
}

#endif