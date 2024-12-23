#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <source_location>
#include <filesystem>

class Logger
{
    public:
        static void error(std::string msg, const std::source_location location = std::source_location::current());
        static void warning(std::string msg, const std::source_location location = std::source_location::current());
        static void message(std::string msg, const std::source_location location = std::source_location::current());
        static void debug(std::string msg, const std::source_location location = std::source_location::current());

        static void init(std::ostream* output);
        static void init(std::filesystem::path fileLocation);
        static void deinit();

    private:
        static Logger& getInstance();
        static void log(std::string msg,
         const std::source_location location =
               std::source_location::current());

        static inline std::ostream* logFile = &std::cout;
        static inline std::ofstream* file = nullptr;
};

#endif