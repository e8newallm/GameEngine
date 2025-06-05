#include <filesystem>
#include <ios>
#include <iostream>
#include <source_location>
#include <fstream>
#include <sstream>

#include "logging.h"

namespace GameEng
{
    void Logger::log(const std::string& msg)
    {
        std::string fullMsg = msg + "\r\n";
        logFile->write(fullMsg.c_str(), static_cast<std::streamsize>(fullMsg.length()));
        logFile->flush();
    }

    void Logger::init(std::ostream* output)
    {
        logFile = output;
        atexit(deinit);
    }

    void Logger::init(const std::filesystem::path& fileLocation)
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
