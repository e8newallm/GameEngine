#include <cstdint>
#include <string>

class Version
{
    public:
        Version(uint8_t Major, uint8_t Minor, uint8_t Patch) : Major(Major), Minor(Minor), Patch(Patch) {};
        constexpr uint32_t getVersion() { return (Major << 16) + (Minor << 8) + Patch; };
        constexpr std::string getVersionStr() { return std::to_string(Major) + "." + std::to_string(Minor) + "." + std::to_string(Patch); };

        bool compatible(Version other) { return Major == other.Major && Minor >= other.Minor; };

    private:
        uint8_t Major, Minor, Patch;
};