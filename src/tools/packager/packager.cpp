#include <iterator>
#include <new>
#include <stdio.h>
#include <assert.h>
#include <cstdint>

#include <algorithm>
#include <string>
#include <filesystem>
#include <vector>

#include "zlib.h"
#include "packager.h"
#include "geerror.h"
#include "logging.h"

#define CHUNK (256 * 1024)

PackageManager::PackageManager(std::string file) :
    packageFile(file)
    ,fileList({})
{
    if(!std::filesystem::exists(file))
        throw GameEngineException(GEError::FILE_NOT_FOUND, "File " + file + " not found");

    FILE* openFile = fopen(file.c_str(), "rb");
    if(openFile == NULL)
    {
        fclose(openFile);
        throw GameEngineException(GEError::FILE_NOT_FOUND, "Could not open " + file);
    }
    fseek(openFile, 0L, SEEK_SET);

    //Get the size of the header data
    std::vector<uint8_t> data;
    data.resize(8);
    uint64_t readSize = fread(data.data(), 1, 8, openFile);
    if(readSize != 8)
    {
        fclose(openFile);
        throw GameEngineException(GEError::INVALID_FILE_FORMAT, "Could not read header size of " + file);
    }
    dataStart = byteToNum(data);

    //Reading the header data of the package
    try{ data.resize(dataStart - 8); }
    catch(std::bad_alloc const& err){
        throw GameEngineException(GEError::INVALID_FILE_FORMAT, "Unable to allocate " + std::to_string(dataStart - 8) + " bytes for " + file);
    }
    readSize = fread(data.data(), 1, dataStart - 8, openFile);
    if(readSize != dataStart - 8)
    {
        fclose(openFile);
        throw GameEngineException(GEError::INVALID_FILE_FORMAT, "Could not read entire header of " + file);
    }

    fileList = headerDecompress(data);
    fclose(openFile);
}

std::vector<std::string> PackageManager::getFileList()
{
    std::vector<std::string> fileListStr;
    for(FileEntry entry : fileList) fileListStr.push_back(entry.name);
    return fileListStr;
}

std::vector<uint8_t> PackageManager::getFile(std::string path)
{
    for(FileEntry file : fileList)
    {
        if(file.name == path)
        {
            std::vector<uint8_t> data;
            data.reserve(file.length);
            FILE* package = fopen(packageFile.c_str(), "rb");
            if (!package)
            {
                throw GameEngineException(GEError::FILE_IO, "Could not open package: " + packageFile);
            }
            fseek(package, dataStart + file.start, SEEK_SET);

            int ret;
            unsigned have;
            z_stream strm;
            unsigned char in[CHUNK];
            unsigned char out[CHUNK];

            /* allocate inflate state */
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.avail_in = 0;
            strm.next_in = Z_NULL;
            ret = inflateInit(&strm);
            if (ret != Z_OK)
            {
                fclose(package);
                throw GameEngineException(GEError::FILE_IO, "Could not initialise file inflation");
            }
            /* decompress until deflate stream ends or end of file */
            do {
                strm.avail_in = fread(in, 1, CHUNK, package);
                if (ferror(package)) {
                    (void)inflateEnd(&strm);
                    fclose(package);
                    throw GameEngineException(GEError::FILE_IO, "Could not read from package: " + packageFile);
                }
                if (strm.avail_in == 0)
                    break;
                strm.next_in = in;

                /* run inflate() on input until output buffer not full */
                do {
                    strm.avail_out = CHUNK;
                    strm.next_out = out;
                    ret = inflate(&strm, Z_NO_FLUSH);
                    assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
                    switch (ret) {
                    case Z_NEED_DICT:
                        ret = Z_DATA_ERROR;     /* and fall through */
                        __attribute__ ((fallthrough));
                    case Z_DATA_ERROR:
                    case Z_MEM_ERROR:
                        (void)inflateEnd(&strm);
                        fclose(package);
                        throw GameEngineException(GEError::FILE_IO, "Could not inflate file from package: " + path);
                    }
                    have = CHUNK - strm.avail_out;
                    std::copy(out, out+have, std::back_inserter(data));
                } while (strm.avail_out == 0);

                /* done when inflate() says it's done */
            } while (ret != Z_STREAM_END);

            /* clean up and return */
            (void)inflateEnd(&strm);
            fclose(package);
            return data;
        }
    }
    throw GameEngineException(GEError::FILE_NOT_FOUND, "File " + path + " not found in package " + packageFile);
    return {};
}

std::string PackageManager::getPackageName() { return packageFile; }

std::vector<std::string> getFileList(std::string directory)
{
    std::vector<std::string> list;
    try
    {
        for (const auto & entry : std::filesystem::recursive_directory_iterator(directory))
        {
            if(!entry.is_directory())
            {
                std::string filename = entry.path();
                filename.erase(0, directory.length());
                list.push_back(filename);
            }
        }
        return list;
    }
    catch(std::filesystem::filesystem_error const& err)
    {
        throw GameEngineException(GEError::FILE_NOT_FOUND, "Could not get file list of " + directory + ": " + err.code().message());
    }
}

std::string fileCompress(std::string file)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    FILE* source = fopen(file.c_str(), "rb");
    if(!source)
    {
        throw GameEngineException(GEError::FILE_IO, "Could not open file to compress: " + file);
    }
    std::string compressedData = "";

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, 9);
    if (ret != Z_OK)
    {
        throw GameEngineException(GEError::FILE_IO, "Could not initialise deflation in zlib: " + std::to_string(ret));
    }

    do
    {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source))
        {
            (void)deflateEnd(&strm);
            throw GameEngineException(GEError::FILE_IO, "Could not read file to compress: " + file);
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        do
        {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            compressedData += std::string((char*)&out, have);
        } while (strm.avail_out == 0);
    } while(flush != Z_FINISH);

    (void)deflateEnd(&strm);
    fclose(source);
    return compressedData;
}

void numToByte(std::vector<uint8_t>& data, uint64_t value)
{
    for(int i = 0; i <= 7; i++)
    {
        data.push_back(value & 0xff);
        value = value >> 8;
    }
}

uint64_t byteToNum(std::vector<uint8_t>& data)
{
    assert(data.size() >= 8);
    uint64_t result = 0;
    for(int i = 7; i >= 0; i--)
    {
        result *= 0x100;
        result += data.back();
        data.pop_back();
    }
    return result;
}

std::vector<uint8_t> headerCompress(std::vector<FileEntry> fileList)
{
    std::vector<uint8_t> headData;
    for(FileEntry data : fileList)
    {
        //Make strings start with a null for reverse parsing
        headData.push_back('\0');
        std::copy(data.name.begin(), data.name.end(), std::back_inserter(headData));
        numToByte(headData, data.start);
        numToByte(headData, data.compressedLength);
        numToByte(headData, data.length);
    }
    return headData;
}

std::vector<FileEntry> headerDecompress(std::vector<uint8_t> data)
{
    std::vector<FileEntry> fileList;
    while(data.size() > 0)
    {
        FileEntry newEntry;
        newEntry.length = byteToNum(data);
        newEntry.compressedLength = byteToNum(data);
        newEntry.start = byteToNum(data);
        for(std::vector<uint8_t>::iterator pos = data.end()-1; pos >= data.begin(); pos--)
        {
            if(*pos == '\0')
            {
                newEntry.name = std::string(pos+1, data.end());
                data.erase(pos, data.end());
                break;
            }
            else if(pos == data.begin()) throw GameEngineException(GEError::INVALID_FILE_FORMAT, "Header data appears corrupted");
        }
        fileList.push_back(newEntry);
    }

    //Reverse fileList so it's in the same order as it was at creation
    std::reverse(fileList.begin(), fileList.end());
    return fileList;
}

int dataCompress(std::string directory, std::string file)
{
    FILE* compData = std::tmpfile();

    //Populate file list for compression
    std::vector<std::string> list = getFileList(directory);
    std::vector<FileEntry> dataHeader;
    long currentPosition = 0;
    for(std::string filename : list)
    {
        std::string compressedData = fileCompress(directory + filename);
        FileEntry newData;
        newData.name = filename;
        newData.length = std::filesystem::file_size(directory + filename);
        newData.start = currentPosition;
        newData.compressedLength = compressedData.length();
        currentPosition += newData.compressedLength;
        fwrite(compressedData.c_str(), 1, compressedData.length(), compData);
        dataHeader.push_back(newData);
    }

    //Write header data to file
    FILE* openFile = fopen(file.c_str(), "wb");
    if(!openFile)
    {
        fclose(compData);
        throw GameEngineException(GEError::FILE_IO, "Could not open package file: " + file);
    }
    std::vector<uint8_t> header = headerCompress(dataHeader);
    std::vector<uint8_t> fullHeader;
    uint64_t size = header.size() + 8;
    numToByte(fullHeader, size);
    std::copy(header.begin(), header.end(), std::back_inserter(fullHeader));
    assert(fullHeader.size() == size);
    size_t writeCount = fwrite(fullHeader.data(), 1, fullHeader.size(), openFile);
    if(writeCount != fullHeader.size())
    {
        fclose(openFile);
        fclose(compData);
        throw GameEngineException(GEError::FILE_IO, "Could not write entire header to package file: " + file);
    }

    //Write compressed files to file
    std::fseek(compData, 0, SEEK_SET);
    unsigned char buffer[CHUNK];
    size_t readSize = CHUNK;
    while(readSize == CHUNK)
    {
        readSize = fread(&buffer, 1, CHUNK, compData);
        size_t writeCount = fwrite(buffer, 1, readSize, openFile);
        if(writeCount != readSize)
        {
            fclose(openFile);
            fclose(compData);
            throw GameEngineException(GEError::FILE_IO, "Could not write entire compressed data to package file: " + file);
        }
    }
    fclose(openFile);
    fclose(compData);
    return 0;
}