#include <filesystem>
#include <iostream>
#include <source_location>
#include <fstream>

#include "logging.h"

void Logger::error(std::string msg, const std::source_location location)
{
    log("ERRO: " + msg, location);
}

void Logger::warning(std::string msg, const std::source_location location)
{
    log("WARN: " + msg, location);
}

void Logger::message(std::string msg, const std::source_location location)
{
    log("INFO: " + msg, location);
}

void Logger::debug(std::string msg, const std::source_location location)
{
#ifdef DEBUG
    log("DEBUG: " + msg, location);
#else
    (void)msg;
    (void)location;
#endif
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

Logger& Logger::getInstance()
{
    static Logger log;
    return log;
}

void Logger::log(std::string msg, const std::source_location location)
{
    std::string fullMsg = msg + "\r\n";
#ifdef DEBUG
    fullMsg = std::string("(") + location.function_name() + ": line " + std::to_string(location.line()) + ") " + fullMsg;
#else
    (void)location;
#endif
    logFile->write(fullMsg.c_str(), fullMsg.length());
    logFile->flush();
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
    std::cout << "\r\n" << std::flush;

    logFile = &std::cout;
}