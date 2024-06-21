#include <iterator>
#include <stdio.h>
#include <assert.h>
#include <cstdint>

#include <algorithm>
#include <string>
#include <filesystem>
#include <vector>
#include <iostream>

#include "zlib.h"
#include "packager.h"

#define CHUNK (256 * 1024)

PackageManager::PackageManager(std::string file) :
    packageFile(file)
{
    FILE* openFile = fopen(file.c_str(), "rb");
    fseek(openFile, 0L, SEEK_SET);
    if(openFile == NULL)
    {
        fclose(openFile);
        throw;
    }
    std::vector<uint8_t> data;
    data.resize(8);
    uint64_t readSize = fread(data.data(), 1, 8, openFile);
    dataStart = byteToNum(data);
    data.resize(dataStart - 8);
    readSize = fread(data.data(), 1, dataStart - 8, openFile);
    if(readSize != dataStart - 8)
    {
        fclose(openFile);
        throw;
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
                throw;
            }
            /* decompress until deflate stream ends or end of file */
            do {
                strm.avail_in = fread(in, 1, CHUNK, package);
                if (ferror(package)) {
                    (void)inflateEnd(&strm);
                    fclose(package);
                    throw;
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
                        throw;
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
    throw;
    return {};
}

std::string PackageManager::getPackageName() { return packageFile; }

std::vector<std::string> getFileList(std::string directory)
{
    std::vector<std::string> list;
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

std::string fileCompress(std::string file)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    FILE* source = fopen(file.c_str(), "rb");
    std::string compressedData = "";

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, 9);
    if (ret != Z_OK)
    {}    //return ret;

    do
    {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source))
        {
            (void)deflateEnd(&strm);
            //return Z_ERRNO;
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
        }
        fileList.push_back(newEntry);
    }
    std::reverse(fileList.begin(), fileList.end());
    return fileList;
}

int dataCompress(std::string directory, std::string file)
{
    FILE* compData = std::tmpfile();

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
    std::vector<uint8_t> header = headerCompress(dataHeader);
    std::vector<uint8_t> fullHeader;
    uint64_t size = header.size() + 8;
    numToByte(fullHeader, size);
    std::copy(header.begin(), header.end(), std::back_inserter(fullHeader));
    assert(fullHeader.size() == size);
    fwrite(fullHeader.data(), 1, fullHeader.size(), openFile);

    //Write compressed files to file
    std::fseek(compData, 0, SEEK_SET);
    unsigned char buffer[CHUNK];
    int readSize = CHUNK;
    while(readSize == CHUNK)
    {
        readSize = fread(&buffer, 1, CHUNK, compData);
        fwrite(buffer, 1, readSize, openFile);
    }
    fclose(openFile);
    fclose(compData);
    return 0;
}