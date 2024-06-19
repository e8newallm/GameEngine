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

class PackageManager
{
    public:
        PackageManager(std::string file);

        std::vector<std::string> getFileList();
        std::vector<uint8_t> getFile(std::string path);

    private:
        std::string packageFile;
        std::vector<FileEntry> fileList;
        uint64_t dataStart;
};

std::vector<std::string> getFileList(std::string directory);
std::string fileCompress(std::string file);
void numToByte(std::vector<uint8_t>& data, uint64_t value);
uint64_t byteToNum(std::vector<uint8_t>& value);

std::vector<uint8_t> headerCompress(std::vector<FileEntry> fileList);
std::vector<FileEntry> headerDecompress(std::vector<uint8_t> data);

int dataCompress(std::string directory, std::string file);