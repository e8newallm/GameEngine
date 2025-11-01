#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <iostream>

namespace GameEng
{
/**
 * \brief A class for handling logging of the game.
 *
 */
class Logger
{
  public:
    /**
     * \brief Logs an error in the log.
     *
     * \param args The arguments to be written to the log
     */
    template <typename... Args> static void error(Args... args)
    {
        log("ERROR: " + stringify(args...));
    }

    /**
     * \brief Logs an warning in the log.
     *
     * \param args The arguments to be written to the log
     */
    template <typename... Args> static void warning(Args... args)
    {
        log("WARN: " + stringify(args...));
    }

    /**
     * \brief Logs an message in the log.
     *
     * \param args The arguments to be written to the log
     */
    template <typename... Args> static void message(Args... args)
    {
        log("INFO: " + stringify(args...));
    }

    /**
     * \brief Logs an debug message in the log.
     *
     * \param args The arguments to be written to the log
     */
    template <typename... Args> static void debug(Args... args)
    {
#ifdef DEBUG
        log("DEBUG: " + stringify(args));
#else
        (void)sizeof...(args);
#endif
    }

    /**
     * \brief Converts a parameter pack into a string
     *
     * \param args The arguments to be stringified
     */
    template <typename... Args> static std::string stringify(Args... args)
    {
        std::ostringstream str;
        ((str << args), ...);
        return str.str();
    }

    /**
     * \brief Initialises the log to output to a given ostream, replacing
     * std::cout. Init also registers deinit function to run on game close.
     * \param output The ostream to write log messages to instead of std::cout.
     */
    static void init(std::ostream* output);

    /**
     * \brief Initialises the log to output to a given file location, replacing
     * std::cout. Init also registers deinit function to run on game close.
     * \param output The file location to write log messages to instead of
     * std::cout.
     */
    static void init(const std::filesystem::path& fileLocation);

    /**
     * \brief Deinitialises the logger to close a file being logged to and resets
     * the ostream to std::cout.
     *
     */
    static void deinit();

  private:
    static void log(const std::string& msg);

    static inline std::ostream* logFile = &std::cout;
    static inline std::ofstream* file = nullptr;
};
} // namespace GameEng

#endif
