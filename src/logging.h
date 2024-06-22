#include <iostream>
#include <source_location>

class Logger
{
    public:
        static void error(std::string msg, const std::source_location location = std::source_location::current());
        static void warning(std::string msg, const std::source_location location = std::source_location::current());
        static void message(std::string msg, const std::source_location location = std::source_location::current());
        static void init(std::ostream* output);

    private:
        static Logger& getInstance();
        void log(std::string msg,
         const std::source_location location =
               std::source_location::current());
        Logger();

        static Logger* logFile;
        std::ostream* output;
};