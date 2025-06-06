#include <cstdint>
#include <string>

namespace GameEng
{
/**
 * \brief A class for tracking and comparing version.
 *
 */
class Version
{
  public:
    /**
     * \brief Construct a new Version from individual values.
     *
     * \param Major The major of the version.
     * \param Minor The minor of the version.
     * \param Patch The patch of the version.
     */
    Version(uint8_t Major, uint8_t Minor, uint8_t Patch) : Major(Major), Minor(Minor), Patch(Patch) {};

    /**
     * \brief Construct a new Version from a 32 bit form.
     *
     * \param version
     */
    Version(uint32_t version)
        : Major(version >> 16), Minor((version >> 8) & 0xFF),
          Patch(version & 0xFF) {}; // NOLINT(readability-magic-numbers)

    /**
     * \brief Get the Version in 32 bit form.
     *
     * \return constexpr uint32_t The version in 32 bit form.
     */
    [[nodiscard]] constexpr uint32_t getVersion() const
    {
        return (Major << 16) + (Minor << 8) + Patch;
    }; // NOLINT(readability-magic-numbers)

    /**
     * \brief Get the Version in string form.
     *
     * \return constexpr std::string The version in string form.
     */
    [[nodiscard]] constexpr std::string getVersionStr() const
    {
        return std::to_string(Major) + "." + std::to_string(Minor) + "." + std::to_string(Patch);
    };

    /**
     * \brief Check whether two versions are compatible together.
     *
     * \param other The other version to compare to.
     * \return bool Are the two versions compatible?
     */
    [[nodiscard]] bool compatible(Version other) const
    {
        return Major == other.Major && Minor >= other.Minor;
    };

  private:
    uint8_t Major, Minor, Patch;
};
} // namespace GameEng