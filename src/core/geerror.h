#ifndef GEERROR_H
#define GEERROR_H

#include <exception>
#include <string>

enum class GEError {
    NO_ERROR = 0,
    FILE_NOT_FOUND = 1,
    FILE_IO = 2,
    INVALID_FILE_FORMAT = 3,
};

class GameEngineException : public std::exception
{
    public:
        GameEngineException(GEError errorCode, const std::string& errorMessage);

        const char* what() const noexcept override;
        GEError code() const noexcept;

    private:
        GEError errorCode;
        std::string errorMessage;
};

#endif