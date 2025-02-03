#ifndef PACKAGE_H
#define PACKAGE_H

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
        explicit PackageManager(const std::string& file);

        std::vector<std::string> getFileList();
        std::vector<uint8_t> getFile(const std::string& path);
        const std::string& getPackageName() const;

    private:
        std::string packageFile;
        std::vector<FileEntry> fileList;
        uint64_t dataStart;
};

std::vector<std::string> getFileList(const std::string& directory);
std::string fileCompress(const std::string& file);
void numToByte(std::vector<uint8_t>& data, uint64_t value);
uint64_t byteToNum(std::vector<uint8_t>& data);
std::string getExtension(const std::string& filename);

std::vector<uint8_t> headerCompress(const std::vector<FileEntry>& fileList);
std::vector<FileEntry> headerDecompress(std::vector<uint8_t> data);

int dataCompress(const std::string& directory, const std::string& file);

#endif