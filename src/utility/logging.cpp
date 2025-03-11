#include <filesystem>
#include <iostream>
#include <source_location>
#include <fstream>

#include "logging.h"

namespace GameEng
{
    void Logger::error(const std::string& msg, const std::source_location location)
    {
        log("ERROR: " + msg, location);
    }

    void Logger::warning(const std::string& msg, const std::source_location location)
    {
        log("WARN: " + msg, location);
    }

    void Logger::message(const std::string& msg, const std::source_location location)
    {
        log("INFO: " + msg, location);
    }

    void Logger::debug(const std::string& msg, const std::source_location location)
    {
    #ifdef DEBUG
        log("DEBUG: " + msg, location);
    #else
        (void)msg;
        (void)location;
    #endif
    }

    void Logger::log(const std::string& msg, const std::source_location location)
    {
        std::string fullMsg = msg + "\r\n";
    #ifdef DEBUG
        fullMsg = std::string("(") + location.file_name() + ":" + std::to_string(location.line()) + " " + location.function_name() + ") " + fullMsg;
    #else
        (void)location;
    #endif
        logFile->write(fullMsg.c_str(), fullMsg.length());
        logFile->flush();
    }

    void Logger::init(std::ostream* output)
    {
        logFile = output;
        atexit(deinit);
    }

    void Logger::init(std::filesystem::path fileLocation)
    {
        file = new std::ofstream(fileLocation);
        logFile = file;
        atexit(deinit);
    }

    void Logger::deinit()
    {
        logFile->flush();
        if(file != nullptr)
        {
            file->close();
            delete file;
            file = nullptr;
        }

        logFile = &std::cout;
    }
}