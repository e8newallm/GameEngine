#include <string>
#include <cstdint>
#include <vector>

struct FileEntry
{
    std::string name;
    uint64_t start;
    uint64_t compressedLength;
    uint64_t length;
};

std::string numToStr(long long value);
long long strToNum(std::string value);

std::vector<std::string> getFileList(std::string directory);
std::string fileCompress(std::string file);
int dataCompress(std::string directory, std::string file);