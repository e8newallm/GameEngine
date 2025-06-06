#ifndef GEERROR_H
#define GEERROR_H

#include <exception>
#include <string>

namespace GameEng
{
/**
 * \enum GEError
 * \brief Error codes for GameEngine exceptions.
 */
enum class GEError
{                       // NOLINT(performance-enum-size)
    GPUDEVICE_NOT_INIT, ///< GPUDeviceInit was not run before attempting to use GPUDevice::getGPU()

    FILE_NOT_FOUND,      ///< File does not exist.
    FILE_IO,             ///< File was unable to be written to or read.
    INVALID_FILE_FORMAT, ///< Input file was not formatted correctly.

    STORE_ENTRY_NOT_FOUND, ///< Datastore does not contain an entry of that name.
};

/**
 * \brief Exceptions for use by the game engine.
 */
class GameEngineException : public std::exception
{
  public:
    /**
     * \brief Construct a new Game Engine Exception object.
     *
     * \param errorCode The GEError code of the exception.
     * \param errorMessage A description of what caused the exception.
     */
    GameEngineException(GEError errorCode, std::string errorMessage);

    /**
     * \brief Get the description of the exception that occurred.
     *
     * \return const char* The string description.
     */
    [[nodiscard]] const char* what() const noexcept override;

    /**
     * \brief Get the GEError code of the exception.
     *
     * \return GEError The GEError code.
     */
    [[nodiscard]] GEError code() const noexcept;

  private:
    GEError errorCode;
    std::string errorMessage;
};
} // namespace GameEng

#endif