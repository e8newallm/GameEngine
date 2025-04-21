#include "geerror.h"

#include <utility>

namespace GameEng
{
    GameEngineException::GameEngineException(GEError errorCode, std::string errorMessage) :
        errorCode(errorCode)
        ,errorMessage(std::move(errorMessage))
    {
    }

    const char* GameEngineException::what() const noexcept
    {
        return errorMessage.c_str();
    };

    GEError GameEngineException::code() const noexcept
    {
        return errorCode;
    }
}