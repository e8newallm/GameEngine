#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include <cstdint>
#include <vector>

namespace Packager
{
    /**
    * \brief An file entry in a package.
    *
    */
    struct FileEntry
    {
        std::string name; //< The name of the file.
        uint64_t start; //< The start position of the file in the package.
        uint64_t compressedLength; //< The compressed file length.
        uint64_t length; //< The The file length when uncompressed.
    };

    /**
    * \brief An object that handles reading files out of a package.
    *
    */
    class PackageManager
    {
        public:
            /**
            * \brief Construct a PackageManager object with a linked package.
            *
            * \param file The file location of the linked package.
            */
            explicit PackageManager(const std::string& file);

            /**
            * \brief Get a list of files in the linked package.
            *
            * \return std::vector<std::string> A vector of all the files in the package.
            */
            std::vector<std::string> getFileList();

            /**
            * \brief Gets an uncompressed copy of a file from the package.
            *
            * \param path The path and filename of the file inside the package.
            * \return std::vector<uint8_t> The resulting data.
            */
            std::vector<uint8_t> getFile(const std::string& path);

            /**
            * \brief Gets the filename of the package.
            * Used for error reporting to track which package caused the error.
            *
            * \return const std::string& The filename of the package.
            */
            const std::string& getPackageName() const;

        private:
            std::string packageFile;
            std::vector<FileEntry> fileList;
            uint64_t dataStart;
    };

    /**
    * \brief Loads and compresses a file using zlib.
    * 
    * \param file The file to load and compress.
    * \return std::string The resulting compressed data.
    */
    std::string fileCompress(const std::string& file);

    /**
    * \brief Builds a package header from a file list and compresses it into a concise format.
    * Does not use zlib compression to keep things simple.
    * \param fileList The list of files that have been compressed.
    * \return std::vector<uint8_t> The compressed header.
    */
    std::vector<uint8_t> headerCompress(const std::vector<FileEntry>& fileList);

    /**
    * \brief Decompresses the header and returns a vector of FileEntrys.
    * 
    * \param data The compressed header as a byte blob.
    * \return std::vector<FileEntry> the resulting package file information.
    */
    std::vector<FileEntry> headerDecompress(std::vector<uint8_t> data);

    /**
    * \brief Creates a new package with the contents of a directory.
    * 
    * \param directory The directory to compress the contents of.
    * \param file The location to save the resulting package.
    */
    void createPackage(const std::string& directory, const std::string& file);
}

/**
* \brief Get a list of files in a given directory.
*
* \param directory The directory to get a list of files for.
* \return std::vector<std::string> A vector of all the files in the directory.
*/
std::vector<std::string> getDirContents(const std::string& directory);

/**
* \brief Breaks a 64 bit value down into 8 bytes and pushes them to the end of a byte vector.
* Bytes are pushed in little endian.
* \param data The vector to push the bytes to.
* \param value The value to be broken down.
*/
void numToByte(std::vector<uint8_t>& data, uint64_t value);

/**
* \brief Pops 8 bytes from the end of the vector and reads them into a 64 bit value.
*
* \param data The vector to read the bytes from.
* \return uint64_t The resulting 64 bit value.
*/
uint64_t byteToNum(std::vector<uint8_t>& data);

/**
* \brief Gets the extension of a given filename.
*
* \param filename The filename.
* \return std::string The extension of the filename.
*/
std::string getExtension(const std::string& filename);

#endif