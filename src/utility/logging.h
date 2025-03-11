#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <source_location>
#include <filesystem>

namespace GameEng
{
    class Logger
    {
        public:
            static void error(const std::string& msg, const std::source_location location = std::source_location::current());
            static void warning(const std::string& msg, const std::source_location location = std::source_location::current());
            static void message(const std::string& msg, const std::source_location location = std::source_location::current());
            static void debug(const std::string& msg, const std::source_location location = std::source_location::current());

            static void init(std::ostream* output);
            static void init(std::filesystem::path fileLocation);
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