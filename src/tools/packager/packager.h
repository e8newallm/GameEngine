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

std::vector<std::string> getFileList(std::string directory);
std::string fileCompress(std::string file);
void numToByte(std::vector<uint8_t>& data, uint64_t value);
uint64_t byteToNum(std::vector<uint8_t>& value);

std::vector<uint8_t> headerCompress(std::vector<FileEntry> fileList);
std::vector<FileEntry> headerDecompress(std::vector<uint8_t> data);

std::string fileCompress(std::string file);
int dataCompress(std::string directory, std::string file);