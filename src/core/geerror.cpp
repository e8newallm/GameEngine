#include "geerror.h"

GameEngineException::GameEngineException(GEError errorCode, const std::string& errorMessage) :
    errorCode(errorCode)
    ,errorMessage(errorMessage)
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