#include <iostream>
#include <source_location>
#include <numeric>

#include "logging.h"

void Logger::error(std::string msg, const std::source_location location)
{
    getInstance().log("ERRO: " + msg, location);
}

void Logger::warning(std::string msg, const std::source_location location)
{
    getInstance().log("WARN: " + msg, location);
}

void Logger::message(std::string msg, const std::source_location location)
{
    getInstance().log("INFO: " + msg, location);
}

void Logger::debug(std::string msg, const std::source_location location)
{
#ifdef DEBUG
    getInstance().log("DEBUG: " + msg, location);
#else
    (void)msg;
    (void)location;
#endif
}

void Logger::init(std::ostream* output)
{
    getInstance().output = output;
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
    output->write(fullMsg.c_str(), fullMsg.length());
    output->flush();
}

Logger::Logger()
{
    this->output = &std::cout;
}
